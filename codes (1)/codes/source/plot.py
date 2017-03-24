import matplotlib.pyplot as plt
x = [1, 2, 3, 4, 5, 6]
y = [[[12.856436, 11.007255, 9.540899, 9.488153, 8.33774, 5.567443], \
    [26.896166, 23.822773, 22.082138, 20.574663, 19.373165, 19.520458], \
    [10.282382, 9.40781, 8.929489, 8.109936, 7.709956, 6.469745], \
    [12.031492, 10.241568, 8.607851, 7.288485, 5.931015, 3.987796]], \
    [[3.550832, 2.956304, 2.701244, 2.700668, 2.635547, 2.898844], \
    [8.617038, 7.652361, 6.93176, 6.514763, 6.389064, 6.172175], \
    [5.078816, 4.37164, 4.319808, 4.072477, 3.645107, 3.314146], \
    [3.836367, 3.734805, 3.389301, 3.21594, 3.177565, 3.072172]], \
    [[10.911726, 10.494322, 10.349368, 10.285501, 10.268179, 10.705922], \
    [17.554899, 16.89176, 17.0399, 17.092342, 16.920269, 16.827961], \
    [9.462219, 9.367809, 9.101021, 8.823316, 8.976033, 8.900675], \
    [9.912912, 9.42293, 8.869487, 8.629513, 8.453541, 8.269806]], \
    [[15.770162, 14.043764, 11.640632, 9.109529, 6.851249, 4.460389], \
    [33.191261, 31.455303, 29.240585, 27.122072, 24.299231, 17.718058], \
    [12.140005, 9.683006, 6.828096, 5.262043, 3.91298, 2.503881], \
    [13.464084, 11.786095, 9.810773, 7.779111, 6.004102, 2.956982]]]

labels = ['8K', '16K', '32K', '64K', '128K','1M']

plt.figure(1)                # the first figure
plt.subplot(221)             # the first subplot in the first figure
plt.plot(x, y[0][0], 'rs-', label = 'PAgA2')
plt.plot(x, y[0][1], 'go-', label = 'Perceptron')
plt.plot(x, y[0][2], 'b^-', label = 'GShare')
plt.plot(x, y[0][3], 'yx-', label = 'Alpha')
# plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
#            ncol=2, mode="expand", borderaxespad=0.)
plt.legend(ncol=2, fontsize=10)
plt.xticks(x, labels)
plt.title('DIST-INT-1')
plt.margins(0.2)


plt.subplot(222)             # the second subplot in the first figure
plt.plot(x, y[1][0], 'rs-', label = 'PAgA2')
plt.plot(x, y[1][1], 'go-', label = 'Perceptron')
plt.plot(x, y[1][2], 'b^-', label = 'GShare')
plt.plot(x, y[1][3], 'yx-', label = 'Alpha')
# plt.legend()
plt.xticks(x, labels)
plt.title('DIST-FP-1')
plt.margins(0.2)


plt.subplot(223)             # the third subplot in the first figure
plt.plot(x, y[2][0], 'rs-', label = 'PAgA2')
plt.plot(x, y[2][1], 'go-', label = 'Perceptron')
plt.plot(x, y[2][2], 'b^-', label = 'GShare')
plt.plot(x, y[2][3], 'yx-', label = 'Alpha')
# plt.legend()
plt.xticks(x, labels)
plt.title('DIST-MM-1')
plt.margins(0.2)


plt.subplot(224)             # the fourth subplot in the first figure
plt.plot(x, y[3][0], 'rs-', label = 'PAgA2')
plt.plot(x, y[3][1], 'go-', label = 'Perceptron')
plt.plot(x, y[3][2], 'b^-', label = 'GShare')
plt.plot(x, y[3][3], 'yx-', label = 'Alpha')
# plt.legend()
plt.xticks(x, labels)
plt.title('DIST-SERV-1')
plt.margins(0.2)
# plt.subplots_adjust(bottom=0.15)
# # Pad margins so that markers don't get clipped by the axes
# plt.margins(0.2)
# # Tweak spacing to prevent clipping of tick-labels
# plt.subplots_adjust(bottom=0.15)
plt.show()

# import matplotlib.pyplot as plt
# plt.figure(1)                # the first figure
# plt.subplot(211)             # the first subplot in the first figure
# plt.plot([1, 2, 3])
# plt.subplot(212)             # the second subplot in the first figure
# plt.plot([4, 5, 6])
#
#
# plt.figure(2)                # a second figure
# plt.plot([4, 5, 6])          # creates a subplot(111) by default
#
# plt.figure(1)                # figure 1 current; subplot(212) still current
# plt.subplot(211)             # make subplot(211) in figure1 current
# plt.title('Easy as 1, 2, 3') # subplot 211 title
#
# import numpy as np
# import matplotlib.pyplot as plt
#
# # evenly sampled time at 200ms intervals
# t = np.arange(0., 5., 0.2)
#
# # red dashes, blue squares and green triangles
# plt.plot(t, t, 'r--', t, t**2, 'bs', t, t**3, 'g^')
# plt.show()

# import numpy as np
# import matplotlib.pyplot as plt
# list_t = np.arange(0, T, 1)
# plt.plot(list_t, S, label = "S(t)")
# plt.legend()
# plt.xlabel('t')
# plt.show()
