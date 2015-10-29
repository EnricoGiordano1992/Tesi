package gui;

import javax.swing.ImageIcon;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JButton;
import javax.swing.JSpinner;
import javax.swing.JSlider;
import javax.swing.JCheckBox;
import javax.swing.JLabel;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.io.IOException;

import javax.swing.event.ChangeListener;
import javax.swing.event.ChangeEvent;

public class SensorsControlPanel extends JPanel {
	private JTextField textField;
	private JTextField textField_1;
	private JTextField textField_2;
	private JTextField textField_3;
	private JTextField textField_4;
	private JTextField textField_5;
	private JTextField textField_6;

	private GUI g;
	/**
	 * Create the panel.
	 */
	public SensorsControlPanel(GUI g) {
		this.g = g;
		
		setLayout(null);
		
		textField = new JTextField();
		textField.setBounds(173, 70, 122, 28);
		add(textField);
		textField.setColumns(10);
		
		textField_1 = new JTextField();
		textField_1.setBounds(173, 138, 122, 28);
		add(textField_1);
		textField_1.setColumns(10);
		
		textField_2 = new JTextField();
		textField_2.setBounds(173, 210, 122, 28);
		add(textField_2);
		textField_2.setColumns(10);
		
		textField_3 = new JTextField();
		textField_3.setBounds(173, 282, 122, 28);
		add(textField_3);
		textField_3.setColumns(10);
		
		JButton btnX = new JButton(new ImageIcon(new ImageIcon(SensorsControlPanel.class.getResource("/javax/swing/plaf/metal/icons/ocean/close.gif")).getImage().getScaledInstance(30, 30, 0)));
		btnX.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					g.showMenu();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnX.setBounds(0, 0, 30, 30);
		add(btnX);
		
		textField_4 = new JTextField();
		textField_4.setBounds(441, 70, 122, 28);
		add(textField_4);
		textField_4.setColumns(10);
		
		textField_5 = new JTextField();
		textField_5.setBounds(441, 138, 122, 28);
		add(textField_5);
		textField_5.setColumns(10);
		
		textField_6 = new JTextField();
		textField_6.setBounds(441, 210, 122, 28);
		add(textField_6);
		textField_6.setColumns(10);
		
		JSpinner spinner = new JSpinner();
		spinner.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				JSpinner source = (JSpinner) e.getSource();
				int val = (int)source.getValue();
				try {
					g.changeMaxTemp(val);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		spinner.setBounds(486, 282, 77, 28);
		add(spinner);
		
		JSlider slider = new JSlider();
		slider.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent arg0) {
				JSlider source = (JSlider) arg0.getSource();
		        double val = (double)source.getValue();
		        try {
					g.changeDelay(val);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
		slider.setBounds(95, 368, 200, 21);
		add(slider);
		
		JCheckBox chckbxNewCheckBox = new JCheckBox("activate alarm on temperature");
		chckbxNewCheckBox.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				JCheckBox source = (JCheckBox) arg0.getSource();
				boolean val = (boolean) source.isSelected();
				try {
					g.setAlarmTempAction(val);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
		chckbxNewCheckBox.setBounds(440, 336, 200, 18);
		add(chckbxNewCheckBox);
		
		JCheckBox chckbxNewCheckBox_1 = new JCheckBox("turn on all leds where there is no light in the area");
		chckbxNewCheckBox_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox source = (JCheckBox) e.getSource();
				boolean val = (boolean) source.isSelected();
				try {
					g.setAlarmLightAction(val);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		chckbxNewCheckBox_1.setBounds(440, 368, 297, 18);
		add(chckbxNewCheckBox_1);
		
		JCheckBox chckbxNewCheckBox_2 = new JCheckBox("activate alarm on presence");
		chckbxNewCheckBox_2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JCheckBox source = (JCheckBox) e.getSource();
				boolean val = (boolean) source.isSelected();
				try {
					g.setAlarmPresenceAction(val);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		chckbxNewCheckBox_2.setBounds(440, 400, 188, 18);
		add(chckbxNewCheckBox_2);
		
		JLabel lblNewLabel = new JLabel("temperature:");
		lblNewLabel.setBounds(80, 76, 70, 16);
		add(lblNewLabel);
		
		JLabel lblNewLabel_1 = new JLabel("humidity:");
		lblNewLabel_1.setBounds(80, 144, 55, 16);
		add(lblNewLabel_1);
		
		JLabel lblNewLabel_2 = new JLabel("presence:");
		lblNewLabel_2.setBounds(80, 216, 55, 16);
		add(lblNewLabel_2);
		
		JLabel lblNewLabel_3 = new JLabel("light:");
		lblNewLabel_3.setBounds(80, 288, 55, 16);
		add(lblNewLabel_3);
		
		JLabel lblSound = new JLabel("sound:");
		lblSound.setBounds(363, 76, 55, 16);
		add(lblSound);
		
		JLabel lblDistance = new JLabel("distance:");
		lblDistance.setBounds(363, 144, 55, 16);
		add(lblDistance);
		
		JLabel lblNewLabel_4 = new JLabel("vibration:");
		lblNewLabel_4.setBounds(363, 216, 55, 16);
		add(lblNewLabel_4);
		
		JLabel lblDelayOfRefresh = new JLabel("delay of refresh:");
		lblDelayOfRefresh.setBounds(95, 337, 104, 16);
		add(lblDelayOfRefresh);
		
		JLabel lblMaxTemperature = new JLabel("max temperature:");
		lblMaxTemperature.setBounds(363, 288, 111, 16);
		add(lblMaxTemperature);

	}
	
	
}
