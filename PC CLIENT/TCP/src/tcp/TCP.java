package tcp;

// TCPClient.java
// A client program implementing TCP socket
import java.net.*; 
import java.io.*; 

public class TCP { 
	public void tcp() 
	{// arguments supply message and hostname of destination  
		Socket s = null; 
		try{ 
			int serverPort = 23;
			String ip = "192.168.10.20";
			String init = "\r\n";
			String data = "window_1\r\n";
			String digit;
			String st;

			s = new Socket(ip, serverPort); 
			DataInputStream input = new DataInputStream( s.getInputStream()); 
			DataOutputStream output = new DataOutputStream( s.getOutputStream()); 


			digit = input.readLine();
			st = new String(digit);
			System.out.println(st); 

			output.writeBytes(data); // UTF is a string encoding

			digit = input.readLine();
			st = new String(digit);
			System.out.println(st); 

		}
		catch (UnknownHostException e){ 
			System.out.println("Sock:"+e.getMessage());}
		catch (EOFException e){
			System.out.println("EOF:"+e.getMessage()); }
		catch (IOException e){
			System.out.println("IO:"+e.getMessage());} 
		finally {
			if(s!=null) 
				try {s.close();
				} 
			catch (IOException e) {/*close failed*/}
		}
	}
}
