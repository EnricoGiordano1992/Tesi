package app;

import gui.GUI;
import tcp.*;

import java.awt.EventQueue;

public class Main {

	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					TCP tcp = new TCP();
					GUI window = new GUI();
					window.setTCP(tcp);
					tcp.setGUI(window);
					
					window.getFrame().setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

}
