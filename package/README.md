Preparing static build
===============

In order to make static build with predefined python path it is recommended to build python from source. Download python source code and place somewhere in a file system. Build python as usual except `configure` step, where you should specify `prefix` and `exec_prefix`:

```
./configure --prefix=/usr/local/share/subutai --exec-prefix=/usr/local/share/subutai
```

Note, that path specified above is for Darwin version of a launcher. On Linux you should use `/opt/subutai` as a default directory.

Next step is to build Launcher with a custom configuration arguments:

```
./configure --static --python=3.7m --python-dir=/usr/local/share/subutai --static-libs=/tmp/launcher
```

* `--static` will force to use static libraries
* `--python` is a version of python we should search for
* `--python-dir` full path to built python
* `--static-libs` is a directory where all static libs will be collected (workaround to darwin)

Packaging for MacOS
===============

Inside darwin directory run ```./pack.sh path-to-launcher path-to-python```

* `path-to-launcher` is a path to launcher binary built for Darwin Platform
* `path-to-python` is a directory where compiled python is located. Note that it should be the same version which launcher was built with

`bomutils` is required to build macos package
