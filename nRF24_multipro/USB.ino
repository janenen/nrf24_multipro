/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License.
 If not, see <http://www.gnu.org/licenses/>.
 */

#define USB_PACKET_PERIOD    400
#define USB_PACKET_SIZE      8

uint32_t process_USB()
{
    uint32_t timeout = micros() + USB_PACKET_PERIOD;
    USB_send_packet(0);
    return timeout;
}

void USB_init()
{
  Gamepad.begin();
}

void USB_send_packet(u8 bind)
{
    union {
        u16 value;
        struct {
            u8 lsb;
            u8 msb;
        } bytes;
    } chanval;

    if (bind) {

    } else {
        chanval.value = map(ppm[AILERON], PPM_MIN, PPM_MAX, 0, 0xFFFF);   // aileron
        packet[0] = chanval.bytes.msb;
        packet[1] = chanval.bytes.lsb;
        chanval.value = map(ppm[ELEVATOR], PPM_MIN, PPM_MAX, 0, 0xFFFF);   // elevator
        packet[2] = chanval.bytes.msb;
        packet[3] = chanval.bytes.lsb;
        chanval.value = map(ppm[THROTTLE], PPM_MIN, PPM_MAX, 0, 0xffff);   // throttle
        packet[4] = chanval.bytes.msb;
        packet[5] = chanval.bytes.lsb;
        chanval.value = map(ppm[RUDDER], PPM_MIN, PPM_MAX, 0, 0xffff);   // rudder
        packet[6] = chanval.bytes.msb;
        packet[7] = chanval.bytes.lsb;
    }

    USB_WritePayload(packet, USB_PACKET_SIZE);
}
void USB_WritePayload(uint8_t *data, uint8_t length){
  Gamepad.yAxis(-(data[0]<<8|data[1])+32767);
  Gamepad.rxAxis((data[2]<<8|data[3])+32768);
  Gamepad.xAxis((data[4]<<8|data[5])+32768);
  Gamepad.zAxis(data[6]+128);
  Gamepad.write();
}


