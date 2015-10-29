package gui;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;

import java.awt.BorderLayout;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JTextField;
import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.JInternalFrame;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import com.jgoodies.forms.factories.DefaultComponentFactory;

import javax.swing.JButton;

import java.awt.Color;
import java.awt.Component;
import java.awt.ComponentOrientation;
import java.awt.Font;
import java.awt.FlowLayout;
import java.io.IOException;

import javax.swing.BoxLayout;
import javax.swing.JTextPane;

import tcp.TCP;

public class GUI {

	private JFrame frame;
	private JPanel actualP;
	private TCP tcp;

	/**
	 * @return the frame
	 */
	public JFrame getFrame() {
		return frame;
	}

	/**
	 * @param frame the frame to set
	 */
	public void setFrame(JFrame frame) {
		this.frame = frame;
	}	


	/**
	 * Create the application.
	 * @param tcp 
	 * @throws UnsupportedLookAndFeelException 
	 * @throws IllegalAccessException 
	 * @throws InstantiationException 
	 * @throws ClassNotFoundException 
	 */
	public GUI(TCP tcp) throws ClassNotFoundException, InstantiationException, IllegalAccessException, UnsupportedLookAndFeelException {
		this.tcp = tcp;
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 * @throws UnsupportedLookAndFeelException 
	 * @throws IllegalAccessException 
	 * @throws InstantiationException 
	 * @throws ClassNotFoundException 
	 */
	private void initialize() throws ClassNotFoundException, InstantiationException, IllegalAccessException, UnsupportedLookAndFeelException {
		UIManager.setLookAndFeel("com.sun.java.swing.plaf.nimbus.NimbusLookAndFeel");
		setFrame(new JFrame());
		getFrame().setResizable(false);
		getFrame().setBounds(100, 100, 800, 500);
		getFrame().setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		getFrame().getContentPane().setLayout(new BorderLayout(0, 0));
		actualP = new MenuPanel(this);
		getFrame().getContentPane().add(actualP, BorderLayout.CENTER);
	}
	

	/******************************************************
	 * 
	 * JPanel caricati Runtime
	 * 
	 * 
	 */

	
	public void showMenu() throws IOException{
		tcp.sendExitController();
		getFrame().remove(actualP);
		actualP = new MenuPanel(this);
		getFrame().getContentPane().add(actualP, BorderLayout.CENTER);
		refresh();
	}
	
	public void switchToLedsControl() throws IOException{
		tcp.sendSwitchContextLeds();
		getFrame().remove(actualP);
		actualP = new LedsControlPanel(this);
		getFrame().getContentPane().add(actualP, BorderLayout.CENTER);
		refresh();
	}
	
	public void switchToSensorsControl() throws IOException{
		tcp.sendSwitchContextSensors();
		getFrame().remove(actualP);
		actualP = new SensorsControlPanel(this);
		getFrame().getContentPane().add(actualP, BorderLayout.CENTER);
		refresh();
	}

	private void refresh(){
		getFrame().validate();
		getFrame().repaint();
	}

	
	/******************************************************
	 * 
	 * Metodi per chiamare operazioni esterne alla GUI
	 * [OUTPUT]
	 * @throws IOException 
	 */
	
	public void ledPowerAction(int led) throws IOException{
		tcp.sendLedPowerAction(led);
	}
	
	public void changeDelay(double delay) throws IOException{
		tcp.sendNewDelay(delay);
	}
	
	public void changeMaxTemp(int temp) throws IOException{
		tcp.sendNewMaxTemp(temp);
	}
	
	public void setAlarmTempAction(boolean check) throws IOException{
		tcp.sendNewAlarmTemp(check);
	}
	
	public void setAlarmLightAction(boolean check) throws IOException{
		tcp.sendNewAlarmLight(check);
	}
	
	public void setAlarmPresenceAction(boolean check) throws IOException{
		tcp.sendNewAlarmPresence(check);
	}

	
	/******************************************************
	 * 
	 * Metodi per chiamare operazioni interne alla GUI
	 * [INPUT]
	 */
	
	public void setLedsGUI(){
		
	}
	
	public void setSensorsGUI(){
		
	}

	public void changeDelayFromExt(double delay){
		
	}
	
	public void changeMaxTempFromExt(int temp){
		
	}
	
	public void setAlarmTempActionFromExt(boolean check){
		
	}
	
	public void setAlarmLightActionFromExt(boolean check){
	
	}
	
	public void setAlarmPresenceActionFromExt(boolean check){
		
	}
}
