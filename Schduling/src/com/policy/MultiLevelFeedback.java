package com.policy;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class MultiLevelFeedback {
	List<Process> processList;
	private int timeQuantum1;
	private int timeQuantum2;
	private int count;
	
	int j=0;
	private int ganntP[];
	private int ganntT[];
	private int totalWaitingTime = 0;
	private int totalTurnAroundTime = 0;

	private float avgWatingTime = 0;
	private float avgTurnaroundTime = 0;

	MultiLevelFeedback(List<Process> processList, int timeQuantum1,int timeQuantum2) {
		count = processList.size();
		ganntT=new int[200];
		ganntP=new int[200];
		this.timeQuantum1 = timeQuantum1;
		this.timeQuantum2 = timeQuantum2;
		this.processList=new ArrayList<Process>();
		for(Process p : processList)
		{
			this.processList.add(new Process(p.getProcessId(), p.getArrivalTime(), p.getBurstTime(),p.getPriority()));
		}
		

		Collections.sort(this.processList, Process.BY_ARRIVAL_TIME);
		

	}

	public void simulate() {
		
		int currentTime =0;
		int remainingProcess = count;
		while (remainingProcess > processList.size()/2) 
	{
			int clockTime=currentTime;
			for (int i = 0; i < count; i++) 
			{
				Process current = processList.get(i);
				if(currentTime<current.getArrivalTime())
					break;
				if (current.getStartTime() == -1)
					current.setStartTime(currentTime);
				ganntP[j]=current.getProcessId();
				ganntT[j]=currentTime;
				j++;
				if (current.getRemainingTime() <= timeQuantum1 && current.getEndTime()==-1)
				{
					current.setEndTime(currentTime + current.getRemainingTime());
					currentTime += current.getRemainingTime();
					current.setRemainingTime(0);
					remainingProcess--;
				} 
				else if (current.getRemainingTime()>timeQuantum1)
				{
					currentTime += timeQuantum1;
					current.setRemainingTime(current.getRemainingTime()-timeQuantum1);
				}

			}
			if(clockTime==currentTime)
			{
				currentTime++;
			}
		}
		while (remainingProcess > processList.size()/2) 
		{
				int clockTime=currentTime;
				for (int i = 0; i < count; i++) 
				{
					Process current = processList.get(i);
					if(currentTime<current.getArrivalTime())
						break;
					if (current.getStartTime() == -1)
						current.setStartTime(currentTime);
					ganntP[j]=current.getProcessId();
					ganntT[j]=currentTime;
					j++;
					if (current.getRemainingTime() <= timeQuantum2 && current.getEndTime()==-1)
					{
						current.setEndTime(currentTime + current.getRemainingTime());
						currentTime += current.getRemainingTime();
						current.setRemainingTime(0);
						remainingProcess--;
					} 
					else if (current.getRemainingTime()>timeQuantum2)
					{
						currentTime += timeQuantum2;
						current.setRemainingTime(current.getRemainingTime()-timeQuantum2);
					}

				}
				if(clockTime==currentTime)
				{
					currentTime++;
				}
			}			
		for(int i=0;i<count;i++)
		{
			Process current=processList.get(i);
			if(current.getRemainingTime()>0 )
			{	
				if(currentTime<current.getArrivalTime())
				{	
					currentTime=current.getArrivalTime();
					current.setStartTime(currentTime);
				}
			
			current.setEndTime(currentTime+current.getRemainingTime());
			currentTime+=current.getRemainingTime();
			}
		}
			
		
		for (int i = 0; i < count; i++) 
		{
			
			Process current = processList.get(i);
			current.setWaitingTime(current.getEndTime()-current.getBurstTime()-current.getArrivalTime());
			current.setTurnaroundTime(current.getEndTime()	- current.getArrivalTime());

			totalWaitingTime += current.getWaitingTime();
			totalTurnAroundTime += current.getTurnaroundTime();
		}
		avgWatingTime = (float) totalWaitingTime / count;
		avgTurnaroundTime = (float) totalTurnAroundTime / count;
		System.out.print("in simulate");
	}

	public void printResult() 
	{
		System.out.println("Simulation result of MultiLevelFeedback ");
		System.out
				.println("PId ArrivalT BurstT Priority StartT   EndT WaitingT TurnAroundT");
		for (Process p : processList) 
		{
			System.out.println(p);

		}
		System.out.println("Average Waiting Time of MultiLevelFeedback  "
				+ avgWatingTime);
		System.out.println("Average TurnAround Time of MultiLevelFeedback  "
				+ avgTurnaroundTime);
		for(int i=0;i<j;i++)
		{
			System.out.println("time "+ganntT[i]+" process "+ganntP[i]);	
			
		}
		System.out.println();
	}
}
