## Download and setup the client app

1. Clone the client application to local:

   ```bash
   sudo apt-get install git-core

   git clone https://github.com/louisschraeder/iot-azure-c.git
   ```

2. Run setup script:

   ```bash
   cd iot-azure-c

   sudo chmod u+x setup.sh

   sudo ./setup.sh
   ```


## Run your client application
Run the client application with root priviledge, and you also need provide your Azure IoT hub device connection string, note your connection should be quoted in the command.

```bash
sudo ./app '<your Azure IoT hub device connection string>'
```

### Send Cloud-to-Device command
You can send a C2D message to your device. You can see the device prints out the message and blinks once when receiving the message.

### Send Device Method command
You can send `start` or `stop` device method command to your Pi to start/stop sending message to your IoT hub.