package eagle_day_server;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import eagle_day_server.datatypes.PlaneStatus;

public class Eagle_Day_Server {
		
	InetAddress spieler1Addr;
	InetAddress spieler2Addr;
	
	public PlaneStatus planeOne=new PlaneStatus();
	public PlaneStatus planeTwo=new PlaneStatus();
	
	public Eagle_Day_Server() throws IOException {
		System.out.println("[Eagle_Day_Server] Starting...");
		
		DatagramSocket inputConnection1 = new DatagramSocket(Portnummer.EINGABE_S1.PORT);
		
		Thread inputHandlerS1 = new Thread(new InputConnection(inputConnection1,planeOne));
		
		byte[] bbuf=new byte[100];
		DatagramPacket rec = new DatagramPacket(bbuf,100);
		System.out.println("Waiting for Player 1");
		inputConnection1.receive(rec);
		spieler1Addr = rec.getAddress();
		System.out.println("Player 1 Connected");
		inputHandlerS1.start();
		
		System.out.println("Creating Output to Player 1");
		Thread outputHandlerS1 = new Thread(new OutputConnection(spieler1Addr,Portnummer.AUSGABE_S1, planeOne));//WRONG PLANE! Correct: planeTwo
		outputHandlerS1.start();
		
		
		
//.................................................................................................................
		
		
		
		DatagramSocket inputConnection2 = new DatagramSocket(Portnummer.EINGABE_S2.PORT);
		
		Thread inputHandlerS2 = new Thread(new InputConnection(inputConnection2,planeTwo));
		
		byte[] bbuf2=new byte[100];
		DatagramPacket rec2 = new DatagramPacket(bbuf2,100);
		System.out.println("Waiting for Player 2");
		inputConnection2.receive(rec2);	
		spieler2Addr = rec2.getAddress();
		System.out.println("Player 2 Connected");
		inputHandlerS2.start();
		
		System.out.println("Creating Output to Player 2");
		Thread outputHandlerS2 = new Thread(new OutputConnection(spieler2Addr,Portnummer.AUSGABE_S2, planeOne));
		outputHandlerS2.start();
		
		
		
	}
	
	
}
