/*******************************************************************************************/
/* The MIT License (MIT)                                                                   */
/*                                                                                         */
/* Copyright (c) 2014 - Marina High School FIRST Robotics Team 4276 (Huntington Beach, CA) */
/*                                                                                         */
/* Permission is hereby granted, free of charge, to any person obtaining a copy            */
/* of this software and associated documentation files (the "Software"), to deal           */
/* in the Software without restriction, including without limitation the rights            */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell               */
/* copies of the Software, and to permit persons to whom the Software is                   */
/* furnished to do so, subject to the following conditions:                                */
/*                                                                                         */
/* The above copyright notice and this permission notice shall be included in              */
/* all copies or substantial portions of the Software.                                     */
/*                                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR              */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE             */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                  */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,           */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN               */
/* THE SOFTWARE.                                                                           */
/*******************************************************************************************/

/*******************************************************************************************/
/* We are a high school robotics team and always in need of financial support.             */
/* If you use this software for commercial purposes please return the favor and donate     */
/* (tax free) to "Marina High School Educational Foundation"  (Huntington Beach, CA)       */
/*******************************************************************************************/

package frc.robot;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class QTextThreadRunnable implements Runnable {
	
	static InetAddress ipAddressRoboRio = null;

	private DatagramSocket socket;

	@Override
	public void run() {
		try {
			socket = new DatagramSocket(JTargetInfo.textPortRoboRioReceive);
		} catch (SocketException e) {
			e.printStackTrace();
		}
		
		try {
			ipAddressRoboRio = InetAddress.getByName(JTargetInfo.ipAddressRoboRio);
		} catch (UnknownHostException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		DatagramPacket packet = null;
		while (!Main.isShuttingDown) {
			JVideoFrame frm = Main.myFrameQueue_WAIT_FOR_TEXT_CLIENT.dropOlderAndRemoveHead();
			if (frm == null) {
				continue;
			}
			String sMsg = frm.m_targetInfo.numberToText();
			if(packet == null)
			{
				packet = new DatagramPacket(sMsg.getBytes(), sMsg.length(), ipAddressRoboRio, JTargetInfo.textPortRoboRioReceive);
			}
			else
			{
				packet.setData(sMsg.getBytes(), 0, sMsg.length());
			}
			try {
				socket.send(packet);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Main.myFrameQueue_WAIT_FOR_BROWSER_CLIENT.addTail(frm);
		}
	}
}
