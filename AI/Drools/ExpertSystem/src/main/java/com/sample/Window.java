package com.sample;
import javax.swing.JOptionPane;

public class Window {
	
	public Window(){}
	
	public static String askUser(String question, String[] options) {
		int n = JOptionPane.showOptionDialog(
				null,							//parent
				question,						//Dialog message
				"Know your Star Wars planet",		//Dialog title
				JOptionPane.YES_NO_OPTION,		//option type
				JOptionPane.QUESTION_MESSAGE,	//message type
				null,     						//icon
				options,  						//options
				options[0]					//initial value
			); 
			if (n<0 || n >= options.length)
				return "User interruption";
			else 
				return options[n];
		}			
}
