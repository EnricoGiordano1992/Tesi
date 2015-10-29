package gui;

import javax.swing.JPanel;
import javax.swing.JButton;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.io.IOException;

import javax.swing.SwingConstants;
import javax.swing.ImageIcon;
import javax.swing.JTextField;

public class LedsControlPanel extends JPanel {
	private JTextField txtOff;
	private JTextField txtOff_1;
	private JTextField txtOff_2;
	private JTextField txtOff_3;
	private JTextField txtOff_4;
	private JTextField txtOff_5;
	
	private GUI g;

	/**
	 * Create the panel.
	 */
	public LedsControlPanel(GUI g) {
		this.g = g;
		
		setLayout(null);
		
		JButton btnX = new JButton(new ImageIcon(new ImageIcon(LedsControlPanel.class.getResource("/javax/swing/plaf/metal/icons/ocean/close.gif")).getImage().getScaledInstance(30, 30, 0)));
		btnX.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				try {
					g.showMenu();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		});
		btnX.setBounds(0, 0, 30, 30);
		add(btnX);
		
		JButton btnNewButton = new JButton("LED 1");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					g.ledPowerAction(1);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnNewButton.setBounds(164, 55, 120, 60);
		add(btnNewButton);
		
		JButton btnNewButton_1 = new JButton("LED 3");
		btnNewButton_1.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					g.ledPowerAction(3);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnNewButton_1.setBounds(164, 190, 120, 60);
		add(btnNewButton_1);
		
		JButton btnNewButton_2 = new JButton("LED 5");
		btnNewButton_2.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					g.ledPowerAction(5);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnNewButton_2.setBounds(164, 320, 120, 60);
		add(btnNewButton_2);
		
		JButton btnNewButton_3 = new JButton("LED 2");
		btnNewButton_3.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					g.ledPowerAction(2);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnNewButton_3.setBounds(462, 55, 120, 60);
		add(btnNewButton_3);
		
		JButton btnNewButton_4 = new JButton("LED 4");
		btnNewButton_4.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					g.ledPowerAction(4);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnNewButton_4.setBounds(462, 190, 120, 60);
		add(btnNewButton_4);
		
		JButton btnNewButton_5 = new JButton("LED 6");
		btnNewButton_5.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					g.ledPowerAction(6);
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		btnNewButton_5.setBounds(462, 320, 120, 60);
		add(btnNewButton_5);
		
		txtOff = new JTextField();
		txtOff.setText("");
		txtOff.setBounds(200, 121, 43, 28);
		add(txtOff);
		txtOff.setColumns(10);
		
		txtOff_1 = new JTextField();
		txtOff_1.setText("");
		txtOff_1.setBounds(499, 121, 43, 28);
		add(txtOff_1);
		txtOff_1.setColumns(10);
		
		txtOff_2 = new JTextField();
		txtOff_2.setText("");
		txtOff_2.setBounds(200, 255, 43, 28);
		add(txtOff_2);
		txtOff_2.setColumns(10);
		
		txtOff_3 = new JTextField();
		txtOff_3.setText("");
		txtOff_3.setBounds(499, 255, 43, 28);
		add(txtOff_3);
		txtOff_3.setColumns(10);
		
		txtOff_4 = new JTextField();
		txtOff_4.setText("");
		txtOff_4.setBounds(200, 384, 43, 28);
		add(txtOff_4);
		txtOff_4.setColumns(10);
		
		txtOff_5 = new JTextField();
		txtOff_5.setText("");
		txtOff_5.setBounds(499, 384, 43, 28);
		add(txtOff_5);
		txtOff_5.setColumns(10);

	}
	
	
}
