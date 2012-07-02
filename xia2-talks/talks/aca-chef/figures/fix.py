import sys

fix_map = {'Rmerge':'$R_{\\mathrm{merge}}$',
           'Rd':'$R_d$',
           'Rcp':'$R_{cp}$'}

def fix():
    for record in sys.stdin:
        out = record
        for fix in fix_map:
            out = out.replace(fix, fix_map[fix])
        sys.stdout.write(out)

if __name__ == '__main__':
    fix()
