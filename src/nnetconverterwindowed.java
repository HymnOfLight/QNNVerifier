import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JFileChooser;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Component;
import javax.swing.Box;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.BoxLayout;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SpringLayout;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.awt.Choice;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.nio.file.Paths;
import java.awt.event.ActionEvent;
import javax.swing.JLabel;
import javax.swing.JOptionPane;

import java.awt.Window.Type;
import javax.swing.DropMode;

public class nnetconverterwindowed {
	
	private JFrame frmNnetconverter;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					nnetconverterwindowed window = new nnetconverterwindowed();
					window.frmNnetconverter.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public nnetconverterwindowed() {
		initialize();
	}
	private static void showFileOpenDialog(Component parent, JTextArea msgTextArea, JTextArea FilenametextArea) {
		JFileChooser fileChooser = new JFileChooser();
		fileChooser.setCurrentDirectory(new File("."));
		fileChooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
		fileChooser.setMultiSelectionEnabled(true);
		fileChooser.addChoosableFileFilter(new FileNameExtensionFilter("NeuralNetworkModel(*.onnx, *.c, *.pb, *.nnet, *.h5)", "onnx", "pb", "nnet", "h5"));
		fileChooser.setFileFilter(new FileNameExtensionFilter("NeuralNetworkModel(*.onnx, *.c, *.pb, *.nnet, *.h5)", "onnx", "pb", "nnet", "h5"));
		int result = fileChooser.showOpenDialog(parent);
		if (result == JFileChooser.APPROVE_OPTION) {
			File file = fileChooser.getSelectedFile();
			msgTextArea.append("open file: " + file.getAbsolutePath() + "\n\n");
			FilenametextArea.setText(file.getAbsolutePath());
	        }	
	    }
	private static void convertNeuralNetworks(Choice choice,JTextArea msgTextArea,JTextArea FilenametextArea) throws IOException
	{
		String functionname = "";
		String sourcefile = "";
		sourcefile = FilenametextArea.getText();
		functionname = JOptionPane.showInputDialog("please input function name");
		switch (choice.getSelectedIndex()) {
        case 0:  msgTextArea.append(nnetconverter.Keras2c(sourcefile, functionname).toString());
                 break;
        case 1:  msgTextArea.append(nnetconverter.Onnx2keras(sourcefile, functionname).toString());
                 break;
        case 2:  msgTextArea.append(nnetconverter.h52pb(sourcefile, functionname).toString());
                 break;
        case 3:  msgTextArea.append(nnetconverter.pb2nnet(sourcefile, functionname).toString());
                 break;
        case 4:  nnetconverter.insertassertions();
                 break;      
        default: System.out.println("invalid input number: "+choice.getSelectedIndex());
        		
                 break;
		}
	}
	private void initialize() {
		frmNnetconverter = new JFrame();
		frmNnetconverter.setType(Type.UTILITY);
		frmNnetconverter.setTitle("nnetconverter");
		frmNnetconverter.setBounds(100, 100, 480, 360);
		frmNnetconverter.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		SpringLayout springLayout = new SpringLayout();
		frmNnetconverter.getContentPane().setLayout(springLayout);
		JPanel panel = new JPanel();
		
		final JTextArea msgTextArea = new JTextArea(10, 30);
		msgTextArea.setWrapStyleWord(true);
		msgTextArea.setLineWrap(true);
		msgTextArea.setEditable(false);
		
		msgTextArea.setLineWrap(true);
		frmNnetconverter.getContentPane().add(msgTextArea);
		JScrollPane scroll = new JScrollPane (msgTextArea, 
				   JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
		springLayout.putConstraint(SpringLayout.NORTH, scroll, -166, SpringLayout.SOUTH, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.WEST, scroll, -466, SpringLayout.EAST, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.SOUTH, scroll, 0, SpringLayout.SOUTH, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.EAST, scroll, 0, SpringLayout.EAST, frmNnetconverter.getContentPane());
		frmNnetconverter.getContentPane().add(scroll);
		
		JButton openBtn = new JButton("open");
		springLayout.putConstraint(SpringLayout.NORTH, openBtn, 46, SpringLayout.NORTH, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.EAST, openBtn, -10, SpringLayout.EAST, frmNnetconverter.getContentPane());

		JTextArea FilenametextArea = new JTextArea();
		springLayout.putConstraint(SpringLayout.WEST, openBtn, 85, SpringLayout.EAST, FilenametextArea);
		springLayout.putConstraint(SpringLayout.WEST, FilenametextArea, 34, SpringLayout.WEST, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.SOUTH, FilenametextArea, -3, SpringLayout.SOUTH, openBtn);
		springLayout.putConstraint(SpringLayout.EAST, FilenametextArea, 296, SpringLayout.WEST, frmNnetconverter.getContentPane());
		frmNnetconverter.getContentPane().add(FilenametextArea);
				
		openBtn.addActionListener(new ActionListener() {
	            @Override
	            public void actionPerformed(ActionEvent e) {
	                showFileOpenDialog(frmNnetconverter, msgTextArea,FilenametextArea);
	            }
	        });
		frmNnetconverter.getContentPane().add(openBtn);
		
		Choice choice = new Choice();
		springLayout.putConstraint(SpringLayout.WEST, choice, 34, SpringLayout.WEST, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.SOUTH, choice, -196, SpringLayout.SOUTH, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.EAST, choice, -170, SpringLayout.EAST, frmNnetconverter.getContentPane());
		choice.add("keras to C");
		choice.add("onnx to C");
		choice.add("Tensorflow pb to C");
		choice.add("nnet to C");
		frmNnetconverter.getContentPane().add(choice);
		
		
		JButton convertBtn = new JButton("convert");
		springLayout.putConstraint(SpringLayout.WEST, convertBtn, 0, SpringLayout.WEST, openBtn);
		springLayout.putConstraint(SpringLayout.SOUTH, convertBtn, -30, SpringLayout.NORTH, scroll);
		convertBtn.addActionListener(new ActionListener() {
	            @Override
	            public void actionPerformed(ActionEvent e) {
	                try {
						convertNeuralNetworks(choice,msgTextArea,FilenametextArea);
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
	            }
	        });
		frmNnetconverter.getContentPane().add(convertBtn);
		
	
		
		JLabel lblNewLabel = new JLabel("File to Convert:");
		springLayout.putConstraint(SpringLayout.WEST, lblNewLabel, 34, SpringLayout.WEST, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.SOUTH, lblNewLabel, -281, SpringLayout.SOUTH, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.NORTH, FilenametextArea, 5, SpringLayout.SOUTH, lblNewLabel);
		frmNnetconverter.getContentPane().add(lblNewLabel);
		
		JLabel lblNewLabel_1 = new JLabel("Please select the convert mode:");
		springLayout.putConstraint(SpringLayout.NORTH, choice, 6, SpringLayout.SOUTH, lblNewLabel_1);
		springLayout.putConstraint(SpringLayout.SOUTH, lblNewLabel_1, -221, SpringLayout.SOUTH, frmNnetconverter.getContentPane());
		springLayout.putConstraint(SpringLayout.WEST, lblNewLabel_1, 33, SpringLayout.WEST, frmNnetconverter.getContentPane());
		frmNnetconverter.getContentPane().add(lblNewLabel_1);

	}
}
