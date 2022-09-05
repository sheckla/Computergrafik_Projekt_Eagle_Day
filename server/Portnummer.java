package eagle_day_server;

public enum Portnummer {
	EINGABE_S1(19411),
	EINGABE_S2(19412),
	
	AUSGABE_S1(19413),
	AUSGABE_S2(19414);
	
	public final int PORT;
	private Portnummer(int x) {this.PORT=x;}
}
