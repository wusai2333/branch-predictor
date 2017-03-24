# Part of result
"""
PAgA2
budget,  used_bits,  avg_mis_pred_rate,  parameter
8256,    7168,       10.772289,          ('PAgA2', 9, 10, 0, 0, 0, 0)
16512,   15360,      9.625411,           ('PAgA2', 10, 11, 0, 0, 0, 0)
33024,   32768,      8.558036,           ('PAgA2', 11, 12, 0, 0, 0, 0)
66048,   57344,      7.895963,           ('PAgA2', 12, 12, 0, 0, 0, 0)
132096,  122880,     7.023179,           ('PAgA2', 13, 13, 0, 0, 0, 0)
1052672, 819200,     5.908149,           ('PAgA2', 15, 17, 0, 0, 0, 0)
"""
"""
Perceptron
budget,  used_bits,  avg_mis_pred_rate,  parameter
8256,    4616,       21.564841,          ('Perceptron', 6, 0, 275, 8, 9, 8) # theta 275 better
16512,   9224,       19.955549,          ('Perceptron', 7, 0, 275, 8, 9, 8)
33024,   22538,      18.823596,          ('Perceptron', 8, 0, 275, 10, 11, 8)
66048,   45066,      17.825960,          ('Perceptron', 9, 0, 275, 10, 11, 8)
132096,  90122,      16.745432,          ('Perceptron', 10, 0, 275, 10, 11, 8)
1052672, 720906,     15.059663,          ('Perceptron', 13, 0, 275, 10, 11, 8) # theta 60 better -> 14.69
"""
"""
GShare
budget,  used_bits, GHR_length, address_length, avg_mis_pred_rate
8256,    8200,      8,          12,             9.240856
16512,   16394,     10,         13,             8.207566
33024,   32778,     10,         14,             7.294604
66048,   65549,     13,         15,             6.566943
132096,  131087,    15,         16,             6.061019
1052672, 1048595,   19,         19,             5.297112
"""
"""
Alpha
budget,     used_bits,  avg_mis_pred_rate,  parameter
8256,       8201,       9.811214,           ('Alpha', 9, 9, 0, 9, 0, 0)
16512,      14347,      8.796349,           ('Alpha', 9, 9, 0, 11, 0, 0)
33024,      29708,      7.669353,           ('Alpha', 10, 10, 0, 12, 0, 0)
66048,      61453,      6.728262,           ('Alpha', 11, 11, 0, 13, 0, 0)
132096,     126990,     5.891556,           ('Alpha', 12, 12, 0, 14, 0, 0)
1052672,    851984,     4.571689,           ('Alpha', 15, 15, 0, 16, 0, 0)
"""

# Initialization
import commands
budgets = [8192 + 64, 16384 + 128, 32768 + 256, 65536 + 512,
           131072 + 1024, 1048576 + 4096]
trace_files = ['DIST-INT-1', 'DIST-FP-1', 'DIST-MM-1', 'DIST-SERV-1']


# Generate PAgA2 parameters
parameters = []
for (address_length, LHR_length) in \
    [(9, 10), (10, 11), (11, 12), (12, 12), (13, 13), (15, 17)]:
    #[(9, 10)]:
    predictor_type = "PAgA2"
    address_length = address_length
    LHR_length     = LHR_length
    theta          = 0
    GHR_length     = 0
    w_count        = 0
    w_length       = 0
    parameters.append((predictor_type, address_length, LHR_length,
                       theta, GHR_length, w_count, w_length))
PAgA2_parameters = parameters

# Generate Perceptron parameters
# Perceptron constraint: w_count = GHR_length + 1
parameters = []
for (address_length, GHR_length) in \
    [(6, 8), (7, 8), (8, 10), (9, 10), (10, 10), (13, 10)]:
    #[(6, 8)]:
    predictor_type = "Perceptron"
    address_length = address_length
    LHR_length     = 0
    theta          = 275
    GHR_length     = GHR_length
    w_count        = GHR_length + 1
    w_length       = 8
    parameters.append((predictor_type, address_length, LHR_length,
                       theta, GHR_length, w_count, w_length))
Perceptron_parameters = parameters

# Generate G-Share parameters
# G-Share constraint: address_length >= GHR_length
parameters = []
for (address_length, GHR_length) in \
    [(12, 8), (13, 10), (14, 10), (15, 13), (16, 15), (19, 19)]:
    #[(12, 8)]:
    predictor_type = "GShare"
    address_length = address_length
    LHR_length     = 0
    theta          = 0
    GHR_length     = GHR_length
    w_count        = 0
    w_length       = 0
    parameters.append((predictor_type, address_length, LHR_length,
                       theta, GHR_length, w_count, w_length))
GShare_parameters = parameters

# Generate Alpha parameters
parameters = []
for (address_length, GHR_length) in \
    [(9, 9), (9, 11), (10, 12), (11, 13), (12, 14), (15, 16)]:
    #[(9, 9)]:
    predictor_type = "Alpha"
    address_length = address_length
    LHR_length     = address_length
    theta          = 0
    GHR_length     = GHR_length
    w_count        = 0
    w_length       = 0
    parameters.append((predictor_type, address_length, LHR_length,
                       theta, GHR_length, w_count, w_length))
Alpha_parameters = parameters

benchmark_results_all_parameters = []
all_parameters = [PAgA2_parameters, Perceptron_parameters, \
                   GShare_parameters, Alpha_parameters]
for parameters in all_parameters:
    # Initialize benchmark results
    benchmark_results = []
    for i in range(0, len(budgets)):
        used_bits = 0
        avg_mis_pred_rate = float('inf')
        parameter = ()
        files_results = []
        benchmark_results.append((used_bits, avg_mis_pred_rate, parameter, \
                                  files_results))

    # Enumerate all parameters
    for i in range(0, len(parameters)):
        sum_mis_pred_rate = 0
        parameter = parameters[i]
        files_results = []
        for trace_file in trace_files:
            command = ('./predictor ../traces/%s type=%s address_length=%d' + \
                        ' LHR_length=%d theta=%d GHR_length=%d w_count=%d' + \
                        ' w_length=%d') % \
                         ((trace_file,) + parameter);
            (result_code, output) = commands.getstatusoutput(command)
            [used_bits, mis_preds, num_branches, mis_pred_rate] = output.split(',')
            used_bits = int(used_bits)
            mis_preds = int(mis_preds)
            num_branches = int(num_branches)
            mis_pred_rate = float(mis_pred_rate)
            sum_mis_pred_rate += mis_pred_rate
            # Output result of current parameter using current file
            print(output)
            files_results.append((trace_file, mis_pred_rate))

        # Use average misprediction rate to choose proper parameter
        avg_mis_pred_rate = sum_mis_pred_rate / 4.0
        current_result = (used_bits, avg_mis_pred_rate, parameter, files_results)

        # Output result of current parameter
        print('%d/%d' % (i + 1, len(parameters)), current_result)

        for j in range(0, len(budgets)):
            if used_bits < budgets[j]:
                if avg_mis_pred_rate < benchmark_results[j][1]:
                    benchmark_results[j] = current_result;

    # Output results
    print("budget, benchmark_results")
    for i in range(0, len(budgets)):
        budget = budgets[i]
        # (used_bits, avg_mis_pred_rate, parameter, files_results) = \
        #                                             benchmark_results[i]
        print("%d,%s" % (budget, repr(benchmark_results[i])))

    # Save benchmark results
    benchmark_results_all_parameters.append(benchmark_results);

# Plot
y = []
for trace_file_num in range(0, len(trace_files)):               # Different file
    y1 = []
    for predictor_type_num in range(0, len(all_parameters)):    # Different predictor type
        y2 = []
        for budget_num in range(0, len(budgets)):               # Different budget
            benchmark_results = benchmark_results_all_parameters[predictor_type_num]
            (used_bits, avg_mis_pred_rate, parameter, files_results) = \
                    benchmark_results[budget_num]
            (trace_file, mis_pred_rate) = files_results[trace_file_num]
            y2.append(mis_pred_rate)
        y1.append(y2)
    y.append(y1)
print(y)

import matplotlib.pyplot as plt
x = [1, 2, 3, 4, 5, 6]
labels = ['8K', '16K', '32K', '64K', '128K','1M']

plt.figure(1)                # the first figure
plt.subplot(221)             # the first subplot in the first figure
plt.plot(x, y[0][0], 'rs-', label = 'PAgA2')
plt.plot(x, y[0][1], 'go-', label = 'Perceptron')
plt.plot(x, y[0][2], 'b^-', label = 'GShare')
plt.plot(x, y[0][3], 'yx-', label = 'Alpha')
plt.legend()
plt.xticks(x, labels)
plt.title('DIST-INT-1')
plt.subplot(222)             # the second subplot in the first figure
plt.plot(x, y[1][0], 'rs-', label = 'PAgA2')
plt.plot(x, y[1][1], 'go-', label = 'Perceptron')
plt.plot(x, y[1][2], 'b^-', label = 'GShare')
plt.plot(x, y[1][3], 'yx-', label = 'Alpha')
plt.legend()
plt.xticks(x, labels)
plt.title('DIST-FP-1')
plt.subplot(223)             # the third subplot in the first figure
plt.plot(x, y[2][0], 'rs-', label = 'PAgA2')
plt.plot(x, y[2][1], 'go-', label = 'Perceptron')
plt.plot(x, y[2][2], 'b^-', label = 'GShare')
plt.plot(x, y[2][3], 'yx-', label = 'Alpha')
plt.legend()
plt.xticks(x, labels)
plt.title('DIST-MM-1')
plt.subplot(224)             # the fourth subplot in the first figure
plt.plot(x, y[3][0], 'rs-', label = 'PAgA2')
plt.plot(x, y[3][1], 'go-', label = 'Perceptron')
plt.plot(x, y[3][2], 'b^-', label = 'GShare')
plt.plot(x, y[3][3], 'yx-', label = 'Alpha')
plt.legend()
plt.xticks(x, labels)
plt.title('DIST-SERV-1')
plt.show()
