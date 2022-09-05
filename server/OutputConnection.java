package eagle_day_server;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import eagle_day_server.datatypes.PlaneStatus;

public class OutputConnection implements Runnable{

	private PlaneStatus plane;
	private Portnummer PORT;
	private InetAddress targetAddr;
	
	private int trys=3;

	public OutputConnection(InetAddress addr,Portnummer port, PlaneStatus status) {
		this.targetAddr=addr;
		this.PORT=port;
		this.plane=status;
	}

	@Override
	public synchronized void run() {
		try {
			System.out.println("[THREAD OUT] sending to "+targetAddr+" @ "+PORT.PORT);
			DatagramSocket clientSocket = new DatagramSocket();

			for(;;) {
				String message="";
				message+=plane.m_00;
				message+=",";
				message+=plane.m_01;
				message+=",";
				message+=plane.m_02;
				message+=",";
				message+=plane.m_03;
				message+=",";
				
				message+=plane.m_10;
				message+=",";
				message+=plane.m_11;
				message+=",";
				message+=plane.m_12;
				message+=",";
				message+=plane.m_13;
				message+=",";
				
				message+=plane.m_20;
				message+=",";
				message+=plane.m_21;
				message+=",";
				message+=plane.m_22;
				message+=",";
				message+=plane.m_23;
				message+=",";
				
				message+=plane.m_30;
				message+=",";
				message+=plane.m_31;
				message+=",";
				message+=plane.m_32;
				message+=",";
				message+=plane.m_33;
				message+=",";
				
				if(plane.isShooting)
					message+=1;
				else
					message+=0;
				message+=",";
				
				message+=plane.speed;
				message+=",";
				
				message+=plane.enemyHP;
				message+=",";
				
				DatagramPacket package_ = new DatagramPacket(message.getBytes(), message.getBytes().length);
				package_.setAddress(targetAddr);
				package_.setPort(PORT.PORT);
				clientSocket.send(package_);
				
				//System.out.println("[THREAD OUT] "+message);
				
				//Thread.sleep(500);
			}
		}catch (Exception e) {
			System.err.println("[THREAD OUT] "+e);
			trys--;
			if(trys>0)run();
		}
		return;
	}

}
