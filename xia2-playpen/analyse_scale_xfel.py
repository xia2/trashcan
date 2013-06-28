from __future__ import division
import iotbx.phil
from scitbx import matrix
from cctbx.crystal import symmetry
from libtbx.utils import Usage, multi_out
import sys
from xfel.cxi.util import is_odd_numbered # implicit import
from xfel.command_line.cxi_merge import master_phil
from xfel.command_line.cxi_xmerge import xscaling_manager
import numpy

def meansd(values):
    import math

    assert(len(values) > 1)
    
    mean = sum(values) / len(values)
    var = sum([(v - mean) ** 2 for v in values]) / len(values - 1)
    return mean, math.sqrt(var)

def merge(observations):
    result = { }
    for hkl in observations:
        result[hkl] = sum(observations[hkl]) / len(observations[hkl])

    return result

def common_indices(set_a, set_b):
    indices_a = set([d.hkl for d in set_a])
    indices_b = set([d.hkl for d in set_b])

    return indices_a.intersection(indices_b)

def compute_cc(list_a, list_b):

    import math
    
    assert(len(list_a) == len(list_b))
    mean_a = sum(list_a) / len(list_a)
    mean_b = sum(list_b) / len(list_b)

    sum_aa = 0.0
    sum_ab = 0.0
    sum_bb = 0.0

    for a, b in zip(list_a, list_b):
        sum_aa += (a - mean_a) ** 2
        sum_ab += (a - mean_a) * (b - mean_b)
        sum_bb += (b - mean_b) ** 2

    try:
        return sum_ab / math.sqrt(sum_aa * sum_bb)
    except ZeroDivisionError, zde:
        return 0.0

def cc(set_a, set_b):
    merged_a = merge(set_a)
    merged_b = merge(set_b)

    _a = []
    _b = []

    for hkl in merged_a:
        if hkl in merged_b:
            _a.append(merged_a[hkl])
            _b.append(merged_b[hkl])

    if len(_a) == 0:
        return 0, 0.0

    return len(_a), compute_cc(_a, _b)    

def pairwise_product(set_a, set_b):
    merged_a = merge(set_a)
    merged_b = merge(set_b)

    _a = []
    _b = []

    for hkl in merged_a:
        if hkl in merged_b:
            _a.append(merged_a[hkl])
            _b.append(merged_b[hkl])

    if len(_a) == 0:
        return 0, 0.0

    import math

    result = sum([a * b for a, b in zip(_a, _b)]) / math.sqrt(
        sum([a ** 2 for a in _a]) * sum([b ** 2 for b in _b]))

    return len(_a), result

def loop_through_frames(starts, ends, joins = { }):
    for s, e in zip(starts, ends):
        for x in range(s, e):
            yield x
        for _s, _e in joins.get(s, []):
            for x in range(_s, _e):
                yield x

def frame_range_thing(starts, ends, j, joins):
    for x in range(starts[j], ends[j]):
        yield x
    for s, e in joins.get(starts[j], []):
        for x in range(s, e):
            yield x

def merge_frames(starts, ends, joins, i, j):
    '''merge frame j into frame i; update joins table; remove references to 
    frame j; works in place'''

    if not i in joins:
        joins[i] = [(starts[j], ends[j])]
    else:
        joins[i].append((starts[j], ends[j]))
        
    # now fix the starts, ends table

    starts.remove(starts[j])
    ends.remove(ends[j])

    return

def mean(values):
    return sum(values) / len(values)

class Frame:
    '''A class to represent one set of intensity measurements from X-fel
    data collection.'''

    def __init__(self, indices, intensities, sigmas):
        self._raw_indices = list(indices)
        self._raw_intensities = list(intensities)
        self._raw_sigmas = list(sigmas)

        self._intensities = intensities
        self._sigmas = sigmas

        return

    def get_intensities(self):
        return self._intensities
    
    def get_sigmas(self):
        return self._sigmas

    def get_raw_intensities(self):
        return self._raw_intensities
    
    def get_raw_sigmas(self):
        return self._raw_sigmas

    def get_indices(self):
        return self._raw_indices

    def get_intensity_dict(self):
        '''Useful for CC value between frames'''

        result = { }

        for j in range(len(self._raw_indices)):
            hkl = self._raw_indices[j]
            i = self._raw_intensities[j]
            
            if not hkl in result:
                result[hkl] = []

            result[hkl].append(i)

        return result

    def normalize(self):
        '''Set scale: <I> = 1'''

        scale = sum(self._raw_intensities) / len(self._raw_intensities)

        self._intensities = [i / scale for i in self._raw_intensities]
        self._sigmas = [s / scale for s in self._raw_sigmas]

        return

    def cc(self, other):
        
        return cc(self.get_intensity_dict(), other.get_intensity_dict())

    def reindex(self):
        raise RuntimeError, 'implement me'

    def merge(self, other):

        indices = other.get_indices()
        intensities = other.get_raw_intensities()
        sigmas = other.get_raw_sigmas()

        # better scale factor: scale by common observations

        common_hkl = set(self._raw_indices).intersection(set(indices))

        other_common_intensity = []

        for j, hkl in enumerate(indices):
            if hkl in common_hkl:
                other_common_intensity.append(intensities[j])

        self_common_intensity = []

        for j, hkl in enumerate(self._raw_indices):
            if hkl in common_hkl:
                self_common_intensity.append(self._raw_intensities[j])

        scale_other = mean(other_common_intensity) / mean(self_common_intensity)

        scaled_intensities = [i / scale_other for i in intensities]
        scaled_sigmas = [s / scale_other for s in sigmas]

        self._raw_indices += indices
        self._raw_intensities += scaled_intensities
        self._raw_sigmas += scaled_sigmas

        return

    def common(self, other):
        return len(set(self._raw_indices).intersection(
                set(other.get_indices())))

def find_merge_common_images(args):
    phil = iotbx.phil.process_command_line(args = args,
                                           master_string = master_phil).show()
    work_params = phil.work.extract()
    if ("--help" in args) :
        libtbx.phil.parse(master_phil.show())
        return

    if ((work_params.d_min is None) or
        (work_params.data is None) or
        ((work_params.model is None) and
         work_params.scaling.algorithm != "mark1")) :
        raise Usage("cxi.merge "
                    "d_min=4.0 "
                    "data=~/scratch/r0220/006/strong/ "
                    "model=3bz1_3bz2_core.pdb")
    if ((work_params.rescale_with_average_cell) and
        (not work_params.set_average_unit_cell)) :
        raise Usage(
            "If rescale_with_average_cell=True, you must also specify "+
            "set_average_unit_cell=True.")

    # Read Nat's reference model from an MTZ file.  XXX The observation
    # type is given as F, not I--should they be squared?  Check with Nat!
    log = open("%s_%s_scale.log" % (work_params.output.prefix,
                                    work_params.scaling.algorithm), "w")
    out = multi_out()
    out.register("log", log, atexit_send_to=None)
    out.register("stdout", sys.stdout)

    print >> out, "Target unit cell and space group:"
    print >> out, "  ", work_params.target_unit_cell
    print >> out, "  ", work_params.target_space_group

    uc = work_params.target_unit_cell

    miller_set = symmetry(
        unit_cell=work_params.target_unit_cell,
        space_group_info=work_params.target_space_group
        ).build_miller_set(
        anomalous_flag=not work_params.merge_anomalous,
        d_min=work_params.d_min)
    from xfel.cxi.merging.general_fcalc import random_structure
    i_model = random_structure(work_params)

    # ---- Augment this code with any special procedures for x scaling
    scaler = xscaling_manager(
        miller_set=miller_set,
        i_model=i_model,
        params=work_params,
        log=out)
    scaler.read_all()
    print "finished reading"
    sg = miller_set.space_group()
    pg = sg.build_derived_laue_group()
    miller_set.show_summary()

    hkl_asu = scaler.observations["hkl_id"]
    imageno = scaler.observations["frame_id"]
    intensi = scaler.observations["i"]
    sigma_i = scaler.observations["sigi"]
    
    lookup = scaler.millers["merged_asu_hkl"]    

    # construct table of start / end indices for frames: now using Python
    # range indexing

    starts = [0]
    ends = []
    
    for x in xrange(1, len(scaler.observations["hkl_id"])):
        if imageno[x] != imageno[x - 1]:
            ends.append(x)
            starts.append(x)
            
    ends.append(len(scaler.observations["hkl_id"]))

    keep_start = []
    keep_end = []

    for j, se in enumerate(zip(starts, ends)):
        s, e = se
        isig = sum(i / s for i, s in zip(intensi[s:e], sigma_i[s:e])) / (e - s)
        dmin = 100.0
        for x in xrange(s, e):
            d = uc.d(lookup[hkl_asu[x]])
            if d < dmin:
                dmin = d
        if isig > 6.0 and dmin < 3.2:
            keep_start.append(s)
            keep_end.append(e)

    starts = keep_start
    ends = keep_end

    print 'Keeping %d frames' % len(starts)

    frames = []

    for s, e in zip(starts, ends):
        indices = [tuple(lookup[hkl_asu[x]]) for x in range(s, e)]
        intensities = intensi[s:e]
        sigmas = sigma_i[s:e]

        frames.append(Frame(indices, intensities, sigmas))

    while True:

        print 'Analysing %d frames' % len(frames)

        common_reflections = numpy.zeros((len(frames), len(frames)),
                                         dtype = numpy.short)

        obs = { } 

        for j, f in enumerate(frames):
            indices = set(f.get_indices())
            for i in indices:
                _i = tuple(i)
                if not _i in obs:
                    obs[_i] = []
                obs[_i].append(j)

        for hkl in obs:
            obs[hkl].sort()
            for j, f1 in enumerate(obs[hkl][:-1]):
                for f2 in obs[hkl][j + 1:]:
                    common_reflections[(f1, f2)] += 1

        cmn_rfl_list = []

        for f1 in range(len(frames)):
            for f2 in range(f1, len(frames)):
                cmn_rfl_list.append((common_reflections[(f1, f2)], f1, f2))

        cmn_rfl_list.sort()
        cmn_rfl_list.reverse()
    
        joins = []
    
        for n, f1, f2 in cmn_rfl_list:
            _cc = frames[f1].cc(frames[f2])

            if _cc[0] > 20 and _cc[1] > 0.75:
                # print '===> %d %d' % (n, frames[f1].common(frames[f2]))
                print '%4d %.3f' % _cc, f1, f2
                joins.append((f1, f2))

        if not joins:
            print 'No pairs found'
            break

        joins.sort()
        joins.reverse()
        
        for j1, j2 in joins:
            frames[j1].merge(frames[j2])
            frames.remove(frames[j2])
            
        continue

    

    return

            
        
    
if (__name__ == "__main__"):
    sargs = ["d_min=3.0",
             "output.n_bins=25",
             "target_unit_cell=106.18,106.18,106.18,90,90,90",
             "target_space_group=I23",
             "nproc=1",
             "merge_anomalous=True",
             "plot_single_index_histograms=False",
             "scaling.algorithm=mark1",
             "raw_data.sdfac_auto=True",
             "scaling.mtz_file=fake_filename.mtz",
             "scaling.show_plots=True",
             "scaling.log_cutoff=-3.",
             "set_average_unit_cell=True",
             "rescale_with_average_cell=False",
             "significance_filter.sigma=0.5",
             "output.prefix=poly_122_unpolarized_control"
             ]
    result = find_merge_common_images(args=sargs)
      
