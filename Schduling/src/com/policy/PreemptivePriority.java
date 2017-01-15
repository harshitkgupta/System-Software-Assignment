package com.policy;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class PreemptivePriority {

	List<Process> processList;
	private int count;
	private int totalWaitingTime = 0;
	private int totalTurnAroundTime = 0;
	int j=0;
	private int ganntP[];
	private int ganntT[];
	private float avgWatingTime = 0;
	private float avgTurnaroundTime = 0;

	PreemptivePriority(List<Process> processList) {
		count = processList.size();
		ganntT=new int[200];
		ganntP=new int[200];
		this.processList = new ArrayList<Process>();
		for (Process p : processList) {
			this.processList.add(new Process(p.getProcessId(), p
					.getArrivalTime(), p.getBurstTime(), p.getPriority()));
		}
		Collections.sort(this.processList, Process.BY_ARRIVAL_TIME);
		Collections.sort(this.processList, Process.BY_PRIORITY);

	}

	public void simulate() {
		int currentTime = 0;
		int remainingProcess = count;
	
		while (remainingProcess > 0)
		{
			int min=1000;
			int index=-1;
			Process current = null;
			
			for (int i = 0; i < count; i++) 
			{
				current = processList.get(i);
				
				if (current.getRemainingTime() > 0 && current.getPriority()<min &&current.getArrivalTime()<=currentTime )
				{	
					index=i;
					min=current.getPriority();
				}	
			}
			if(index==-1)
			{	currentTime++;
				continue;
			}
			current = processList.get(index);
				if (current.getStartTime()==-1) 
				{
					current.setStartTime(currentTime);
				}
				current.setRemainingTime(current.getRemainingTime() - 1);
				ganntP[j]=current.getProcessId();
				ganntT[j]=currentTime;
				j++;
				currentTime++;
				
				if (current.getRemainingTime() == 0) 
				{
					current.setEndTime(currentTime );

					remainingProcess--;
				}
				
				
			
			
				

			
			
			
		}
		for (int i = 0; i < count; i++) {
			Process current = processList.get(i);
			current.setWaitingTime(current.getEndTime()-current.getBurstTime()-current.getArrivalTime());
			current.setTurnaroundTime(current.getEndTime()	- current.getArrivalTime());

			totalWaitingTime += current.getWaitingTime();
			totalTurnAroundTime += current.getTurnaroundTime();
		}

		avgWatingTime = (float) totalWaitingTime / count;
		avgTurnaroundTime = (float) totalTurnAroundTime / count;

	}

	public void printResult() {
		Collections.sort(this.processList, Process.BY_PROCESSID);
		System.out
				.println("Simulation result of preemptivePPriority Schduling ");
		System.out
				.println("PId ArrivalT BurstT Priority StartT   EndT WaitingT TurnAroundT");
		for (Process p : processList) {
			System.out.println(p);

		}
		System.out.println("Average Waiting Time of preemptivePrioity "
				+ avgWatingTime);
		System.out.println("Average TurnAround Time of preemptivePriority "
				+ avgTurnaroundTime);
		for(int i=0;i<j;i++)
		{
			System.out.println("time "+ganntT[i]+" process "+ganntP[i]);	
			
		}
		System.out.println();
	}
}
