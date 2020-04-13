import matplotlib.pyplot as plt 
import numpy as np
import array as arr 
import matplotlib.cm as cm
from itertools import cycle

my_colors = iter(cm.gnuplot2(np.linspace(0, 1, 8)))

thread_default = np.array([88])
runtime_default = np.array([0.0264344])
threads = np.array([1, 2, 4, 8, 16, 22, 44, 88])
af_none = np.array([0.324832, 0.163288, 0.0842142, 0.0435799, 0.0230997, 0.0194625, 0.0210078, 0.0264214])
af_sockets_false = np.array([0.325076, 0.163253, 0.0838667, 0.0436435, 0.023046, 0.0190545, 0.0206504, 0.0238985])
af_cores_false = np.array([0.324648, 0.16354, 0.0837889, 0.0434537, 0.0231496, 0.0194563, 0.0212046, 0.0237055])
af_threads_false = np.array([0.325007, 0.163234, 0.0840898, 0.0434851, 0.0231797, 0.0194611, 0.0210849, 0.0239731])
af_sockets_true = np.array([0.326206, 0.163408, 0.0837538, 0.0521883, 0.0292147, 0.0231815, 0.0199503, 0.0232362])
af_cores_true = np.array([0.327976, 0.169122, 0.0871447, 0.0451806, 0.0320285, 0.0322104, 0.0166047, 0.0173305])
af_threads_true = np.array([0.328297, 0.232415, 0.122759, 0.0649044, 0.0373159, 0.0332151, 0.0338539, 0.0172679])
af_sockets_close = np.array([0.325741, 0.163393, 0.084575, 0.0519364, 0.0284708, 0.0237974, 0.0195621, 0.0223689])
af_cores_close = np.array([0.329259, 0.170319, 0.0869501, 0.0452468, 0.0320854, 0.0320711, 0.0165903, 0.0173848])
af_threads_close = np.array([0.328378, 0.233555, 0.123023, 0.0647825, 0.0372326, 0.0333464, 0.0337935, 0.0172952])
af_sockets_spread = np.array([0.325002, 0.163351, 0.0848691, 0.0510771, 0.0282606, 0.0225427, 0.018854, 0.023715])
af_cores_spread = np.array([0.328743, 0.16377, 0.0845073, 0.0436862, 0.0229955, 0.0184291, 0.016607, 0.0174623])
af_threads_spread = np.array([0.328196, 0.163857, 0.0848949, 0.0435662, 0.0229174, 0.0183874, 0.0166003, 0.017286])
af_sockets_master = np.array([0.326047, 0.167599, 0.0865001, 0.0449674, 0.0319715, 0.0323596, 0.0353569, 0.0463479])
af_cores_master = np.array([0.328636, 0.232444, 0.253753, 0.247502, 0.255988, 0.270987, 0.270568, 0.356587])
af_threads_master = np.array([0.328282, 0.355004, 0.35829, 0.355691, 0.382691, 0.42029, 0.52729, 0.70129])

runtime_default = 0.324832/runtime_default
af_none = 0.324832/af_none
af_sockets_false = 0.324832/af_sockets_false
af_cores_false = 0.324832/af_cores_false
af_threads_false = 0.324832/af_threads_false
af_sockets_true = 0.324832/af_sockets_true
af_cores_true = 0.324832/af_cores_true
af_threads_true = 0.324832/af_threads_true
af_sockets_close = 0.324832/af_sockets_close
af_cores_close = 0.324832/af_cores_close
af_threads_close = 0.324832/af_threads_close
af_sockets_spread = 0.324832/af_sockets_spread
af_cores_spread = 0.324832/af_cores_spread
af_threads_spread = 0.324832/af_threads_spread
af_sockets_master = 0.324832/af_sockets_master
af_cores_master = 0.324832/af_cores_master
af_threads_master = 0.324832/af_threads_master

fig, ax = plt.subplots()

linecycler = cycle(("-","--","-.",":"))
markercycler = cycle(('p', 'o', 'v', '^', '8', 'd', 'x'))

plt.plot(threads,threads, color='k', label='Ideal scaling', linestyle='-')
#plt.plot(thread_default,runtime_default, color=next(my_colors), marker='x', label='Default setting')
plt.plot(threads,af_threads_spread, color=next(my_colors), marker=next(markercycler), label='threads spread', linestyle=next(linecycler))
plt.plot(threads,af_cores_spread, color=next(my_colors), marker=next(markercycler), label='cores spread', linestyle=next(linecycler))
plt.plot(threads,af_threads_close, color=next(my_colors), marker=next(markercycler), label='threads close', linestyle=next(linecycler))
plt.plot(threads,af_cores_close, color=next(my_colors), marker=next(markercycler), label='cores close', linestyle=next(linecycler))
#plt.plot(threads,af_sockets_false, color=next(my_colors), marker=next(markercycler), label='sockets false', linestyle=next(linecycler))
plt.plot(threads,af_cores_false, color=next(my_colors), marker=next(markercycler), label='cores false', linestyle=next(linecycler))
plt.plot(threads,af_threads_false, color=next(my_colors), marker=next(markercycler), label='threads false', linestyle=next(linecycler))
plt.plot(threads,af_none, color=next(my_colors), marker=next(markercycler),label='OMP_NUM_PROCS only', linestyle=next(linecycler))
#plt.plot(threads,af_sockets_true, color=next(my_colors), marker=next(markercycler), label='sockets true', linestyle=next(linecycler))
#plt.plot(threads,af_cores_true, color=next(my_colors), marker=next(markercycler), label='cores true', linestyle=next(linecycler))
#plt.plot(threads,af_threads_true, color=next(my_colors), marker=next(markercycler), label='threads true', linestyle=next(linecycler))
#plt.plot(threads,af_sockets_close, color=next(my_colors), marker=next(markercycler), label='sockets close', linestyle=next(linecycler))
#plt.plot(threads,af_sockets_spread, color=next(my_colors), marker=next(markercycler), label='sockets spread', linestyle=next(linecycler))
#plt.plot(threads,af_sockets_master, color=next(my_colors), marker=next(markercycler), label='sockets master', linestyle=next(linecycler))
#plt.plot(threads,af_cores_master, color=next(my_colors), marker=next(markercycler), label='cores master', linestyle=next(linecycler))
#plt.plot(threads,af_threads_master, color=next(my_colors), marker=next(markercycler), label='threads master', linestyle=next(linecycler))

axes = plt.gca() # get current axes
axes.set_xlim([1,88])
axes.set_ylim([1,20])

ax.grid()

locs, labels = plt.xticks()
plt.xticks(threads)
locs, labels = plt.yticks()
speedup = np.array([1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20])
plt.yticks(speedup)

plt.xlabel('Number of Threads',fontsize=16)
plt.ylabel('VecAdd Speedup',fontsize=16)

plt.tick_params(labelsize='large')

plt.legend(loc=4,fontsize='medium')
fig.tight_layout()
plt.savefig("OpenMP_Affinity.pdf")
plt.savefig("OpenMP_Affinity.svg")
plt.savefig("OpenMP_Affinity.png", dpi=900)

plt.show()

