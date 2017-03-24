import commands
trace_file = 'DIST-MM-1'
predictor_type = 'GShare'
GHR_length = 5
address_length = 10
# G-Share constraint: address_length >= GHR_length
for (address_length, GHR_length) in [(12, 8), (13, 10), (14, 10), (15, 13), (16, 15), (19, 19)]:
# for address_length in range(10, 20):
#     for GHR_length in range(1, address_length + 1):
    command = './predictor ../traces/%s type=%s GHR_length=%d address_length=%d' % \
        (trace_file, predictor_type, GHR_length, address_length);
    (result_code, output) = commands.getstatusoutput(command)
    print("%s,%s,%d,%d,%s" % \
        (trace_file, predictor_type, GHR_length, address_length, output));
