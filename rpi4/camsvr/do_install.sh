
#!/bin/bash
cd /home/pi/2020_VisionSystem/rpi4
rm -rf release
mkdir release
cd release 
cp -r /home/pi/2020_VisionSystem/rpi4/camsvr /home/pi/2020_VisionSystem/raspi/release
dos2unix /home/pi/2020_VisionSystem/rpi4/release/camsvr/*.sh
sudo chown -R root /home/pi/2020_VisionSystem/rpi4/release/camsvr
sudo chmod u+x /home/pi/2020_VisionSystem/rpi4/release/camsvr/*.sh
cd /home/pi/2020_VisionSystem/rpi4/release/camsvr
ant clean
ant build
sync

