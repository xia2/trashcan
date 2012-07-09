import sys

collecting = False

for record in sys.stdin:
    if record.startswith('For') and record.count('/') == 2:
        collecting = True
        print '\\begin{tabular}{lr@{.}lr@{.}lr@{.}l}'
        continue

    if 'Assuming spacegroup' in record:
        collecting = False
        print '\\end{tabular}'
        continue
    
    if not collecting:
        continue
    
    property = record[:40].strip()
    values = record[40:].split()

    new_values = []

    for v in values:
        if not '.' in v:
            new_values.append('%s.' % v)
        else:
            new_values.append(v)

    values = new_values
                

    if len(values) == 3:
        print '%s & %s & %s & %s & %s & %s & %s \\\\' % \
            (property,
             values[0].split('.')[0],
             values[0].split('.')[1],
             values[1].split('.')[0],
             values[1].split('.')[1],
             values[2].split('.')[0],
             values[2].split('.')[1])
    else:
        print '%s & %s & %s & & & & \\\\' % \
            (property,
             values[0].split('.')[0],
             values[0].split('.')[1])
        
         
    
