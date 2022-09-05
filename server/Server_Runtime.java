package eagle_day_server;

public class Server_Runtime {
	public static void main(String[] args) {
		try {
			new Eagle_Day_Server();
		}catch(Exception e) {
			System.err.println(e);
		}
	}
}
