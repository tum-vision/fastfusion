fastfusion
==========

Volumetric 3D Mapping in Real-Time on a CPU 

This code implements the approach for real-time 3D mapping on a CPU as
described in the following research paper:

http://vision.in.tum.de/_media/spezial/bib/steinbruecker_etal_icra2014.pdf

Volumetric 3D Mapping in Real-Time on a CPU (F. Steinbruecker, J. Sturm, D. Cremers), 
In Int. Conf. on Robotics and Automation, 2014.

![alt tag](http://vision.in.tum.de/_media/data/software/fastfusion_small.png)

Demo video:
http://youtu.be/7s9JePSln-M

```
@string{icra="Int. Conf. on Robotics and Automation"}
@inproceedings{Steinbruecker-etal-icra14,
  author = {F. Steinbruecker and J. Sturm and D. Cremers},
  title = {Volumetric 3D Mapping in Real-Time on a CPU},
  booktitle = icra,
  year = {2014},
  address = {Hongkong, China},
  titleurl = {steinbruecker_etal_icra2014.pdf},
  topic = {3D Reconstruction},
  keywords =  {RGB-D,Fusion,3d-reconstruction}
}
```

Installation
============

    $ git clone https://github.com/tum-vision/fastfusion.git`

    $ cd fastfusion
  
    $ cmake .

    $ make

Preparation of the data
======================

The software takes a text file as input which contains per file
- the camera pose
- the depth image filename
- the color image filename

You can either generate such a file yourself (e.g., by running
Christan Kerl's DVO SLAM:

http://vision.in.tum.de/data/software/dvo

available as open source on our homepage) or you can download 
sequences from the TUM RGB-D benchmark:

http://vision.in.tum.de/data/datasets/rgbd-dataset/

For simplicity, we take a pre-recorded sequence from the TUM
RGB-D benchmark.

    $ mkdir ~/data

    $ cd ~/data

    $ wget http://vision.in.tum.de/rgbd/dataset/freiburg3/rgbd_dataset_freiburg3_long_office_household.tgz

    $ tar xvzf rgbd_dataset_freiburg3_long_office_household.tgz

Now we need to generate the text file. For this, we use the associate.py tool from
the RGB-D benchmark website. We need to run it twice, as we join the
camera poses, the depth image list and the color image list into a single file:

    $ cd ~/fastfusion/

    $ ./associate.py ~/data/rgbd_dataset_freiburg3_long_office_household/groundtruth.txt ~/data/rgbd_dataset_freiburg3_long_office_household/depth.txt > tmp.txt

    $ ./associate.py tmp.txt ~/data/rgbd_dataset_freiburg3_long_office_household/rgb.txt > ~/data/rgbd_dataset_freiburg3_long_office_household/associate.txt

The resulting text file should look as follows:

    $ head ~/data/rgbd_dataset_freiburg3_long_office_household/associate.txt

```
1341847980.790000 -0.6832 2.6909 1.7373 0.0003 0.8617 -0.5072 -0.0145 1341847980.786879 depth/1341847980.786879.png 1341847980.786856 rgb/1341847980.786856.png
1341847980.820100 -0.6821 2.6914 1.7371 0.0003 0.8609 -0.5085 -0.0151 1341847980.822989 depth/1341847980.822989.png 1341847980.822978 rgb/1341847980.822978.png
1341847980.850000 -0.6811 2.6918 1.7371 0.0001 0.8610 -0.5084 -0.0159 1341847980.854690 depth/1341847980.854690.png 1341847980.854676 rgb/1341847980.854676.png
[..]
```

Running the code
================

    $ ./bin/onlinefusion ~/data/rgbd_dataset_freiburg3_long_office_household/associate.txt --thread-fusion

After some debugging output on the console, a window with a 3D viewer should open. To start the 
reconstruction process, press "S". 

If you run the program for the first time, press and hold the CTRL key and turn your scroll wheel. 
This is only needed once to "free" the camera viewpoint. After this, you can pan (right click) and 
rotate (left click) the view as you wish using your mouse.

Further options
===============

```
   ./bin/onlinefusion  [--intrinsics <string>] [--imagescale <float>]
                       [--threshold <float>] [--scale <float>]
                       [--max-camera-distance <float>]
                       [--consistency-checks <int>] [-k <int>] [-e <int>]
                       [-s <int>] [--incremental-meshing] [-c] [-b] [-v]
                       [--thread-image] [--thread-fusion]
                       [--thread-meshing] [-l <string>] [--] [--version]
                       [-h] <string> ...


Where: 

   --intrinsics <string>
     File with Camera Matrix

   --imagescale <float>
     Image Depth Scale

   --threshold <float>
     Threshold

   --scale <float>
     Size of the Voxel

   --max-camera-distance <float>
     Maximum Camera Distance to Surface

   --consistency-checks <int>
     Number of Depth Consistency Checks

   -k <int>,  --imagestep <int>
     Use every kth step

   -e <int>,  --endimage <int>
     Number of the End Image

   -s <int>,  --startimage <int>
     Number of the Start Image

   --incremental-meshing
     Perform incremental Meshing

   -c,  --loopclosures
     Read Multiple Trajectories and perform Loop Closures

   -b,  --buffer
     Buffer all Images

   -v,  --viewer
     Show a Viewer after Fusion

   --thread-image
     Thread reading the Images from Hard Disk

   --thread-fusion
     Thread the Fusion inside the Viewer

   --thread-meshing
     Thread the Meshing inside the Fusion

   -l <string>,  --loadmesh <string>
     Loads this mesh

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <string>  (accepted multiple times)
     The File Names
```
![alt tag](http://vision.in.tum.de/_media/data/software/screenshot_fastfusion.png)

