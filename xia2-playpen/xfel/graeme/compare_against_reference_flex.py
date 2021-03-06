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

if __name__ == '__main__':
    import sys

    if len(sys.argv) == 1:
        raise RuntimeError, '%s reference [spacegroup] [reindex]' % \
              sys.argv[0]

    reference1 = read_reference(sys.argv[1])

    sgtype = None
    cb_ops = []

    for arg in sys.argv[2:]:
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
            
    if not sgtype:
        sgtype = reference1.space_group().type()

    references = [reference1.change_basis(cb_op).map_to_asu() \
                  for cb_op in cb_ops]

    from collections import defaultdict
    ccs = defaultdict(list)
    
    for i, pkl_file in enumerate(sys.stdin):
        pkl = read_pkl(pkl_file.strip())
        pkl = pkl.average_bijvoet_mates()

        if False:
            # have an I/sigma selection of > 3

            i_over_sigi = pkl.data() / pkl.sigmas()
            pkl = pkl.select(i_over_sigi > 3)

        if False:
            # have an I/sigma selection of > 1

            i_over_sigi = pkl.data() / pkl.sigmas()
            pkl = pkl.select(i_over_sigi > 1)
        
        c, n = compare(reference1, pkl, sgtype)
        print '%5d %s %6.3f %5d' % (i, pkl_file.strip(), c, n),
        ccs[0].append(c)
        for j, reference in enumerate(references):
            c, n = compare(reference, pkl, sgtype)
            print '%6.3f %5d' % (c, n),
            ccs[j + 1].append(c)
        print ''

    from matplotlib import pyplot
    
    for j in sorted(ccs):
        pyplot.scatter(range(len(ccs[j])), ccs[j])
    pyplot.savefig('ccs.png')
    pyplot.close()

    from scitbx.array_family import flex
    
    for j in sorted(ccs):
        hist = flex.histogram(flex.double(ccs[j]), n_slots=50)
        pyplot.plot(hist.slot_centers(), hist.slots())
        pyplot.savefig('cc_hist_%d.png' % j)
        pyplot.close()

