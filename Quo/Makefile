default: autobind dynaffinity

libquo/lib/libquo.a:
	./build_libquo.sh

autobind: autobind.c place_report.c libquo/lib/libquo.a
	mpicc autobind.c place_report.c -o autobind -fopenmp -Ilibquo/include  -Llibquo/lib -lquo -Wl,-rpath=libquo/lib

dynaffinity: dynaffinity.c place_report.c libquo/lib/libquo.a
	mpicc dynaffinity.c place_report.c -o dynaffinity -fopenmp -Ilibquo/include  -Llibquo/lib -lquo -Wl,-rpath=libquo/lib

clean:
	rm -f *.o autobind dynaffinity
	#cd libquo; make clean; make distclean
	cd libquo; rm -rf bin lib include ar-lib config.guess config.h.in config.h.in~ config.sub configure test-driver aclocal.m4
	cd libquo; rm -f config/libtool.m4 config/ltoptions.m4 config/ltsugar.m4 config/ltversion.m4 config/lt~obsolete.m4 config.log
