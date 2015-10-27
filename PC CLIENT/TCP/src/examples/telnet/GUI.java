package examples.telnet;

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

import com.jgoodies.forms.factories.DefaultComponentFactory;

import javax.swing.JButton;

import java.awt.Color;
import java.awt.Component;
import java.awt.ComponentOrientation;
import java.awt.Font;
import java.awt.FlowLayout;

import javax.swing.BoxLayout;
import javax.swing.JTextPane;

public class GUI {

	private JFrame frame;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					GUI window = new GUI();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public GUI() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setResizable(false);
		frame.setBounds(100, 100, 800, 500);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.X_AXIS));
		
		JPanel panel = new JPanel();
		frame.getContentPane().add(panel);
		panel.setLayout(null);
		
		JLabel lblNewJgoodiesTitle = DefaultComponentFactory.getInstance().createTitle("House Control with MODBUS");
		lblNewJgoodiesTitle.setBounds(260, 98, 265, 24);
		panel.add(lblNewJgoodiesTitle);
		lblNewJgoodiesTitle.setFont(new Font("Trebuchet MS", Font.BOLD, 20));
		
		JButton btnNewButton = new JButton(new ImageIcon(new ImageIcon(GUI.class.getResource("led_icon.jpg")).getImage().getScaledInstance(86, 86, 0)));
		btnNewButton.setBounds(158, 194, 98, 98);
		btnNewButton.setBackground(Color.green);
		panel.add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton(new ImageIcon(new ImageIcon(GUI.class.getResource("sensor_icon.jpg")).getImage().getScaledInstance(86, 86, 0)));
		btnNewButton_1.setBounds(336, 194, 98, 98);
		btnNewButton_1.setBackground(Color.blue);
		panel.add(btnNewButton_1);
		
		JButton btnNewButton_2 = new JButton(new ImageIcon(new ImageIcon(GUI.class.getResource("debug_icon.jpg")).getImage().getScaledInstance(86, 86, 0)));
		btnNewButton_2.setBounds(513, 194, 98, 98);
		btnNewButton_2.setBackground(Color.orange);
		panel.add(btnNewButton_2);
		
		JLabel lblLedsControl = new JLabel(" LEDS CONTROL");
		lblLedsControl.setBounds(158, 314, 108, 16);
		panel.add(lblLedsControl);
		
		JLabel lblSensorsControl = new JLabel("   SENSORS CONTROL");
		lblSensorsControl.setBounds(319, 314, 135, 16);
		panel.add(lblSensorsControl);
		
		JLabel lblDebugControl = new JLabel("MODBUS DEBUG");
		lblDebugControl.setBounds(513, 314, 115, 16);
		panel.add(lblDebugControl);
	}
}
