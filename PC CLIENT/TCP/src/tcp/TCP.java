package tcp;

// TCPClient.java
// A client program implementing TCP socket
import gui.LedsControlPanel;

import java.net.*; 
import java.io.*; 

public class TCP { 

	private Socket s;
	private String data;
	private DataInputStream input;
	private DataOutputStream output;
	
	private enum COMMAND{
		QUIT,
		LEDS_CONTROLLER,
		SENSORS_CONTROLLER,
		DEBUG_MODBUS_CONTROLLER,
		RETURN_TO_MENU,
		
		INIT_LEDS,
		CHANGE_LED_STATUS_CONTROLLER_1,
		CHANGE_LED_STATUS_CONTROLLER_2,
		CHANGE_LED_STATUS_CONTROLLER_3,
		CHANGE_LED_STATUS_CONTROLLER_4,
		CHANGE_LED_STATUS_CONTROLLER_5,
		CHANGE_LED_STATUS_CONTROLLER_6,
		UPDATE_LEDS_STATUS_CONTROLLER,
		
		INIT_SENSORS,
		CHANGE_DELAY_QUERY_FROM_CONTROLLER_TO_WINDOW,
		NOTIFY_MAX_TEMPERATURE_THRESHOLD_FROM_CONTROLLER_TO_WINDOW,
		NOTIFY_ALARM_TEMPERATURE_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW,
		NOTIFY_ALARM_PRESENCE_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW,
		NOTIFY_NO_LIGHT_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW,
		NULL
	};
	
	public TCP() 
	{// arguments supply message and hostname of destination  
		try{ 
			int serverPort = 23;
			String ip = "192.168.10.20";
			String init = "\r\n";
			String digit;
			String st;

			s = new Socket(ip, serverPort); 
			input = new DataInputStream( s.getInputStream()); 
			output = new DataOutputStream( s.getOutputStream()); 

			digit = read();
			st = new String(digit);
			System.out.println(st); 

		}
		catch (UnknownHostException e){ 
			System.out.println("Sock:"+e.getMessage());}
		catch (EOFException e){
			System.out.println("EOF:"+e.getMessage()); }
		catch (IOException e){
			System.out.println("IO:"+e.getMessage());} 
//		finally {
//			if(s!=null) 
//				try {s.close();
//				} 
//			catch (IOException e) {/*close failed*/}
//		}
	}
	
	
	
	/************************
	 * 
	 * Funzioni di invio 
	 * 
	 */
	
	
	// MENU
	
	public void sendSwitchContextLeds() throws IOException{
		writeCommand(COMMAND.LEDS_CONTROLLER);
	}

	public void sendSwitchContextSensors() throws IOException{
		writeCommand(COMMAND.SENSORS_CONTROLLER);
	}

	public void sendSwitchContextDebug() throws IOException{
		writeCommand(COMMAND.DEBUG_MODBUS_CONTROLLER);
	}

	public void sendExitController() throws IOException{
		writeCommand(COMMAND.RETURN_TO_MENU);
	}

	// LEDS CONTROLLER
	
	public void sendLedPowerAction(int led) throws IOException{
		COMMAND out = COMMAND.NULL;
		switch(led){
			case 1:
				out = COMMAND.CHANGE_LED_STATUS_CONTROLLER_1;
				break;
			case 2:
				out = COMMAND.CHANGE_LED_STATUS_CONTROLLER_2;
				break;
			case 3:
				out = COMMAND.CHANGE_LED_STATUS_CONTROLLER_3;
				break;
			case 4:
				out = COMMAND.CHANGE_LED_STATUS_CONTROLLER_4;
				break;
			case 5:
				out = COMMAND.CHANGE_LED_STATUS_CONTROLLER_5;
				break;
			case 6:
				out = COMMAND.CHANGE_LED_STATUS_CONTROLLER_6;
				break;
		}
		writeCommand(out);
	}

	// SENSORS CONTROLLER
	public void sendNewDelay(int delay) throws IOException {
		writeCommand(COMMAND.CHANGE_DELAY_QUERY_FROM_CONTROLLER_TO_WINDOW, ""+delay);
	}

	public void sendNewMaxTemp(int temp) throws IOException {
		writeCommand(COMMAND.NOTIFY_MAX_TEMPERATURE_THRESHOLD_FROM_CONTROLLER_TO_WINDOW, ""+temp);
	}

	public void sendNewAlarmTemp(boolean check) throws IOException {
		writeCommand(COMMAND.NOTIFY_ALARM_TEMPERATURE_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW);		
	}

	public void sendNewAlarmLight(boolean check) throws IOException {
		writeCommand(COMMAND.NOTIFY_NO_LIGHT_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW);		
	}

	public void sendNewAlarmPresence(boolean check) throws IOException {
		writeCommand(COMMAND.NOTIFY_ALARM_PRESENCE_SENSOR_CHANGED_FROM_CONTROLLER_TO_WINDOW);		
	}
	
	
	
	/************************
	 * 
	 * Funzioni base 
	 * 
	 */

	private String read() throws IOException{
		return input.readLine();
	}
	
	private void writeCommand(COMMAND data) throws IOException{
		System.out.println(data.ordinal()+"\r\n");
		output.writeBytes(data.ordinal()+"\r\n");
	}

	private void writeCommand(COMMAND data, String arg) throws IOException{
		System.out.println(data.ordinal()+" "+arg+"\r\n");
		output.writeBytes(data.ordinal()+" "+arg+"\r\n");
	}

}
