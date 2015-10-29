package gui;

import java.awt.Color;
import java.awt.Font;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.jgoodies.forms.factories.DefaultComponentFactory;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class MenuPanel extends JPanel {

	private GUI g;
	/**
	 * Create the panel.
	 */
	public MenuPanel(GUI g) {
		
		this.g = g;
		setLayout(null);
		
		JLabel lblNewJgoodiesTitle = DefaultComponentFactory.getInstance().createTitle("House Control with MODBUS");
		lblNewJgoodiesTitle.setBounds(260, 98, 265, 24);
		add(lblNewJgoodiesTitle);
		lblNewJgoodiesTitle.setFont(new Font("Trebuchet MS", Font.BOLD, 20));
		
		JButton btnNewButton = new JButton(new ImageIcon(new ImageIcon(MenuPanel.class.getResource("led_icon.jpg")).getImage().getScaledInstance(86, 86, 0)));
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				g.switchToLedsControl();
			}
		});
		btnNewButton.setBounds(158, 194, 98, 98);
		btnNewButton.setBackground(Color.green);
		add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton(new ImageIcon(new ImageIcon(MenuPanel.class.getResource("sensor_icon.jpg")).getImage().getScaledInstance(86, 86, 0)));
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				g.switchToSensorsControl();
			}
		});
		btnNewButton_1.setBounds(336, 194, 98, 98);
		btnNewButton_1.setBackground(Color.blue);
		add(btnNewButton_1);
		
		JButton btnNewButton_2 = new JButton(new ImageIcon(new ImageIcon(MenuPanel.class.getResource("debug_icon.jpg")).getImage().getScaledInstance(86, 86, 0)));
		btnNewButton_2.setBounds(513, 194, 98, 98);
		btnNewButton_2.setBackground(Color.orange);
		add(btnNewButton_2);
		
		JLabel lblLedsControl = new JLabel(" LEDS CONTROL");
		lblLedsControl.setBounds(158, 314, 108, 16);
		add(lblLedsControl);
		
		JLabel lblSensorsControl = new JLabel("   SENSORS CONTROL");
		lblSensorsControl.setBounds(319, 314, 135, 16);
		add(lblSensorsControl);
		
		JLabel lblDebugControl = new JLabel("MODBUS DEBUG");
		lblDebugControl.setBounds(513, 314, 115, 16);
		add(lblDebugControl);
	}
	
}
