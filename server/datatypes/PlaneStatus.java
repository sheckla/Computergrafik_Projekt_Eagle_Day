package eagle_day_server.datatypes;

public class PlaneStatus {

	
	public double m_00=-99999999;public double m_10=-99999999;public double m_20=-99999999;public double m_30=-99999999;
	public double m_01=-99999999;public double m_11=-99999999;public double m_21=-99999999;public double m_31=-99999999;
	public double m_02=-99999999;public double m_12=-99999999;public double m_22=-99999999;public double m_32=-99999999;
	public double m_03=-99999999;public double m_13=-99999999;public double m_23=-99999999;public double m_33=-99999999;
	
	public boolean isShooting=false;
	public double speed=0.0;
	
	public double enemyHP=100;
	
	public void setShooting(boolean s) {isShooting=s;}
}
