package eagle_day_server;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

import eagle_day_server.datatypes.PlaneStatus;

public class InputConnection implements Runnable{
	
	private DatagramSocket connection;
	private PlaneStatus plane;
	
	String str;

	public InputConnection(DatagramSocket con, PlaneStatus status) {this.connection = con;this.plane=status;}

	@Override
	public synchronized void run() {
		while(true) {
			try {
				
				byte[] bbuf=new byte[200];
				DatagramPacket rec = new DatagramPacket(bbuf,200);
				connection.receive(rec);	
				
				str = new String(bbuf, StandardCharsets.UTF_8);
				
				plane.m_00 = Double.parseDouble(str.split(",")[0]);
				plane.m_01 = Double.parseDouble(str.split(",")[1]);
				plane.m_02 = Double.parseDouble(str.split(",")[2]);
				plane.m_03 = Double.parseDouble(str.split(",")[3]);
				
				plane.m_10 = Double.parseDouble(str.split(",")[4]);
				plane.m_11 = Double.parseDouble(str.split(",")[5]);
				plane.m_12 = Double.parseDouble(str.split(",")[6]);
				plane.m_13 = Double.parseDouble(str.split(",")[7]);
				
				plane.m_20 = Double.parseDouble(str.split(",")[8]);
				plane.m_21 = Double.parseDouble(str.split(",")[9]);
				plane.m_22 = Double.parseDouble(str.split(",")[10]);
				plane.m_23 = Double.parseDouble(str.split(",")[11]);
				
				plane.m_30 = Double.parseDouble(str.split(",")[12]);
				plane.m_31 = Double.parseDouble(str.split(",")[13]);
				plane.m_32 = Double.parseDouble(str.split(",")[14]);
				plane.m_33 = Double.parseDouble(str.split(",")[15]);
				
				if(Integer.parseInt(str.split(",")[16].trim()) == 1)
					plane.setShooting(true);
				else
					plane.setShooting(false);
				
				plane.speed = Double.parseDouble(str.split(",")[17]);
				plane.enemyHP = Double.parseDouble(str.split(",")[18]);
				
				//System.out.println(str);
				
				/* other info... */
				
			}catch (Exception e) {
				System.err.println("[THREAD IN] "+e);
				System.out.println("|||||||||||||||||||||||||||||||||||||||||");
				System.out.println(str);
				System.out.println("|||||||||||||||||||||||||||||||||||||||||");
				break;
			}
		}
		return;
	}
	

}
