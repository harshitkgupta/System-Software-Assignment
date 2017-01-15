package com.policy;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;

public class Schduling {
	
	public static void  main(String string[]) {
		
		List<Process> processList= new ArrayList<Process>();
		readDataFromFile(processList);
		
		FIFO fifo=new FIFO(processList);
		fifo.simulate();
		fifo.printResult();
		
		Priority priority=new Priority(processList);
		priority.simulate();
		priority.printResult();
		
		
		
		SJF sjf=new SJF(processList);
		sjf.simulate();
		sjf.printResult();
		
		int timeQuantum=2;
		RoundRobin roundRobin=new RoundRobin(processList,15);
		roundRobin.simulate();
		roundRobin.printResult();
		
		PreemptivePriority preemptivePriority=new PreemptivePriority(processList);
		preemptivePriority.simulate();
		preemptivePriority.printResult();
		
		SRF srf=new SRF(processList);
		srf.simulate();
		srf.printResult();
		
		MultiLevel multiLevel=new MultiLevel(processList, timeQuantum);
		multiLevel.simulate();
		multiLevel.printResult();
		
		PreemptiveMultiLevel preemptiveMultiLevel=new PreemptiveMultiLevel(processList);
		preemptiveMultiLevel.simulate();
		preemptiveMultiLevel.printResult();
		
		
		
	/*	MultiLevelFeedback multiLevelFeedback=new MultiLevelFeedback(processList, timeQuantum,timeQuantum+2);
		multiLevelFeedback.simulate();
		multiLevelFeedback.printResult();*/
		
		PreemptiveMultiLevelFeedback preemptiveMultiLevelFeedback=new PreemptiveMultiLevelFeedback(processList);
		preemptiveMultiLevelFeedback.simulate();
		preemptiveMultiLevelFeedback.printResult();
		
		
		
		
		
	}

	static void readDataFromFile(List<Process> processList) {

		try {
			FileReader fileStream = new FileReader("proc.txt");
			BufferedReader bufferedReader = new BufferedReader(fileStream);
			Process process;
			String line = null;
			String arr[] = null;
			while ((line = bufferedReader.readLine()) != null) {
				arr = line.split(" ");
				process = new Process(Integer.parseInt(arr[0]), Integer.parseInt(arr[1]),Integer.parseInt(arr[2]), Integer.parseInt(arr[3]));
				processList.add(process);
				//System.out.println(process);
				line = null;
				arr = null;
				process = null;
			}
			bufferedReader.close();
			fileStream.close();
		} catch (Exception e) {

			e.printStackTrace();
		} 
	}
}
