# Result
"""
budget,  used_bits, GHR_length, address_length, avg_mis_pred_rate
8256,    8200,      8,          12,             9.240856
16512,   16394,     10,         13,             8.207566
33024,   32778,     10,         14,             7.294604
66048,   65549,     13,         15,             6.566943
132096,  131087,    15,         16,             6.061019
1052672, 1048595,   19,         19,             5.297112
"""
import commands
budgets = [8192 + 64, 16384 + 128, 32768 + 256, 65536 + 512,
           131072 + 1024, 1048576 + 4096]
benchmark_results = [[0,0,0,float('inf')],[0,0,0,float('inf')],
                     [0,0,0,float('inf')],[0,0,0,float('inf')],
                     [0,0,0,float('inf')],[0,0,0,float('inf')]]
trace_files = ['DIST-INT-1', 'DIST-FP-1', 'DIST-MM-1', 'DIST-SERV-1']
predictor_type = 'GShare'
# G-Share constraint: address_length >= GHR_length
GHR_length = 5
address_length = 10
for address_length in range(12, 20):
    for GHR_length in range(5, address_length + 1):
        sum_mis_pred_rate = 0
        for trace_file in trace_files:
            command = './predictor ../traces/%s type=%s GHR_length=%d address_length=%d' % \
                (trace_file, predictor_type, GHR_length, address_length);
            (result_code, output) = commands.getstatusoutput(command)
            [used_bits, mis_preds, num_branches, mis_pred_rate] = output.split(',')
            used_bits = int(used_bits)
            mis_preds = int(mis_preds)
            num_branches = int(num_branches)
            mis_pred_rate = float(mis_pred_rate)
            sum_mis_pred_rate += mis_pred_rate
            print("%s,%s,%d,%d,%f" % \
                (predictor_type, trace_file, GHR_length, address_length, mis_pred_rate));
        avg_mis_pred_rate = sum_mis_pred_rate / 4.0
        print("avg_mis_pred_rate: %f" % avg_mis_pred_rate)
        for i in range(0, len(budgets)):
            if used_bits < budgets[i]:
                if avg_mis_pred_rate < benchmark_results[i][3]:
                    benchmark_results[i] = [GHR_length, address_length, used_bits, avg_mis_pred_rate];
print;
print("budget,used_bits,GHR_length,address_length,avg_mis_pred_rate")
for i in range(0, len(budgets)):
    budget = budgets[i]
    [GHR_length, address_length, used_bits, avg_mis_pred_rate] = benchmark_results[i]
    print("%d,%d,%d,%d,%f" % (budget, used_bits, GHR_length, address_length, avg_mis_pred_rate))
