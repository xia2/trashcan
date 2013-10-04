def read_reference(mtz_file):
    from iotbx import reflection_file_reader
    reflection_file = reflection_file_reader.any_reflection_file(
      file_name=mtz_file)
    reference = None
    for miller_array in reflection_file.as_miller_arrays():
        label = miller_array.info().labels[0].lower()
        if label.find('imean') >= 0:
            reference = miller_array
            break
        
    assert reference is not None
    return reference.map_to_asu()

def read_pkl(pkl_file):
    import cPickle as pickle
    img = pickle.load(open(pkl_file))
    return img['observations'][0].map_to_asu()

def cc(a, b):

    assert(len(a) == len(b))

    from scitbx.array_family import flex
    corr = flex.linear_correlation(flex.double(a), flex.double(b))
    return corr.coefficient()

def compare(a, b, sgtype):

    ind_a = a.indices()
    ind_b = b.indices()

    from cctbx.miller import map_to_asu
    map_to_asu(sgtype, False, ind_a)
    map_to_asu(sgtype, False, ind_b)
    
    _a, _b = a.common_sets(b, assert_is_similar_symmetry=False)

    if _a.size() < 5:
        return 0.0, _a.size()

    return cc(_a.data(), _b.data()), _a.size()

def get_sgtype_cb_ops(argv):
    sgtype = None
    cb_ops = []

    for arg in argv:
        if not sgtype:
            try:
                from cctbx.sgtbx import space_group, space_group_symbols
                sgtype = space_group(space_group_symbols(arg)).type()
                continue
            except RuntimeError, e:
                pass
            
        try:
            from cctbx import sgtbx
            cb_op = sgtbx.change_of_basis_op(arg)
            cb_ops.append(cb_op)
            continue
        except ValueError, e:
            pass

        raise RuntimeError, '%s not understood' % arg

    return sgtype, cb_ops


if __name__ == '__main__':
    import sys

    if len(sys.argv) == 1:
        raise RuntimeError, '%s reference [spacegroup] [reindex]' % \
              sys.argv[0]

    reference1 = read_reference(sys.argv[1])

    sgtype, cb_ops = get_sgtype_cb_ops(sys.argv[2:])
            
    if not sgtype:
        sgtype = reference1.space_group().type()

    references = [reference1.change_basis(cb_op).map_to_asu() \
                  for cb_op in cb_ops]

    from collections import defaultdict

    all_observations = None

    ignored = 0
    original = 0
    alternative = 0

    for i, pkl_file in enumerate(sys.stdin):
        ccs = []
        pkl = read_pkl(pkl_file.strip())
        pkl = pkl.average_bijvoet_mates()
        c, n = compare(reference1, pkl, sgtype)
        ccs.append(c)
        for j, reference in enumerate(references):
            c, n = compare(reference, pkl, sgtype)
            ccs.append(c)

        if max(ccs) - min(ccs) < 0.1:
            print '%5d %s ignored' % (i, pkl_file.strip())
            ignored += 1
            continue

        correct_op = ccs.index(max(ccs))
        if correct_op == 0:
            print '%5d %s h,k,l' % (i, pkl_file.strip())
            data = pkl
            original += 1
            
        else:
            cb_op = cb_ops[correct_op - 1]
            print '%5d %s %s' % (i, pkl_file.strip(), cb_op)
            data = pkl.change_basis(cb_op.inverse()).map_to_asu()
            alternative += 1

        if all_observations is None:
            all_observations = data.deep_copy()
        else:
            if all_observations.is_similar_symmetry(data):
                all_observations = all_observations.concatenate(data)
            
    merged_equivalents = all_observations.merge_equivalents()
    merged_data = merged_equivalents.array()

    print 'Average CC = %.3f N = %d' % compare(reference1, merged_data, sgtype)
    print 'Ignored %d Original %d Alternative %d' % (ignored, original,
                                                     alternative)

    merged_data.as_mtz_dataset(column_root_label="XFEL").mtz_object().write(
        'xfel.mtz')
