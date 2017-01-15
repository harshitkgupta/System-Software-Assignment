package com.policy;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class MultiLevel {

	List<Process> processList;
	List<Process> fprocessList;
	List<Process> bprocessList;
	private int count;
	private int timeQuantum;
	int j=0;
	private int ganntP[];
	private int ganntT[];
	
	private int totalWaitingTime=0;
	private int totalTurnAroundTime=0;
	
	private float avgWatingTime=0;
	private float avgTurnaroundTime=0;
	MultiLevel(List<Process> processList,int timeQuantum)
	{
		count=processList.size();
		this.timeQuantum=timeQuantum;
		this.processList=new ArrayList<Process>();
		this.fprocessList=new ArrayList<Process>();
		this.bprocessList=new ArrayList<Process>();
		float totpriority=0;
		for(Process p : processList)
		{
				totpriority+=p.getPriority();
		}
		totpriority/=(float)count;
		for(Process p : processList)
		{
			
			if(p.getPriority()<totpriority)
				this.fprocessList.add(new Process(p.getProcessId(), p.getArrivalTime(), p.getBurstTime(),p.getPriority()));
			else
				this.bprocessList.add(new Process(p.getProcessId(), p.getArrivalTime(), p.getBurstTime(),p.getPriority()));
			
		}
		
		Collections.sort(fprocessList, Process.BY_ARRIVAL_TIME);
		Collections.sort(bprocessList, Process.BY_ARRIVAL_TIME);
		
	}
	public void simulate()
	{
		
		int currentTime =0;
		int remainingProcess = fprocessList.size();
		while (remainingProcess > 0) 
	{
		
			int clockTime=currentTime;
			for (int i = 0; i < fprocessList.size(); i++) 
			{
				Process current = fprocessList.get(i);
				if(currentTime<current.getArrivalTime())
					break;
				if (current.getStartTime() == -1)
					current.setStartTime(currentTime);
				
				if (current.getRemainingTime() <= timeQuantum && current.getEndTime()==-1)
				{
					current.setEndTime(currentTime + current.getRemainingTime());
					
					currentTime += current.getRemainingTime();
					current.setRemainingTime(0);
					remainingProcess--;
				} 
				else if (current.getRemainingTime()>timeQuantum)
				{
					currentTime += timeQuantum;
					current.setRemainingTime(current.getRemainingTime() - timeQuantum);
				}

			}
			if(clockTime==currentTime)
			{
				currentTime++;
			}
			
		}
		
		for(int i=0;i<bprocessList.size();i++)
		{
			Process current=bprocessList.get(i);
			if(currentTime<current.getArrivalTime())
				currentTime=current.getArrivalTime();
			current.setStartTime(currentTime);
			
			current.setEndTime(current.getStartTime()+current.getBurstTime());
			currentTime+=current.getBurstTime();
		}
		for(int i=0;i<fprocessList.size();i++)
		{	
			processList.add(fprocessList.get(i));
		}
		for(int i=0;i<bprocessList.size();i++)
		{	
			processList.add(bprocessList.get(i));
		}
		for(int i=0;i<count;i++)
		{	
			Process current=processList.get(i);
			current.setWaitingTime(current.getEndTime()-current.getBurstTime()-current.getArrivalTime());
			current.setTurnaroundTime(current.getEndTime()	- current.getArrivalTime());
			
			totalWaitingTime+=current.getWaitingTime();
			totalTurnAroundTime+=current.getTurnaroundTime();
		}
		
		avgWatingTime=(float)totalWaitingTime/count;
		avgTurnaroundTime=(float)totalTurnAroundTime/count;
		
	}
	public void printResult()
	{
		Collections.sort(this.processList, Process.BY_PROCESSID);
		System.out.println("Simulation result of Multilevel Schduling ");
		System.out.println("PId ArrivalT BurstT Priority StartT   EndT WaitingT TurnAroundT");
		for(Process p : processList)
		{
		System.out.println(p);	
		
		}
		System.out.println("Average Waiting Time of Multilevel "+avgWatingTime);
		System.out.println("Average TurnAround Time of Multilevel "+avgTurnaroundTime);
		for(int i=0;i<j;i++)
		{
			System.out.println("time "+ganntT[i]+" process "+ganntP[i]);	
			
		}
		System.out.println();
		}
}
