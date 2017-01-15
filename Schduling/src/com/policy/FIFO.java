package com.policy;



import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class FIFO {
	List<Process> processList;
	private int count;
	int j=0;
	private int ganntP[];
	private int ganntT[];
	private int totalWaitingTime=0;
	private int totalTurnAroundTime=0;
	
	private float avgWatingTime=0;
	private float avgTurnaroundTime=0;
	FIFO(List<Process> processList)
	{
		ganntT=new int[200];
		ganntP=new int[200];
		count=processList.size();
		this.processList=new ArrayList<Process>();
		for(Process p : processList)
		{
			this.processList.add(new Process(p.getProcessId(), p.getArrivalTime(), p.getBurstTime(),p.getPriority()));
		}
		Collections.sort(this.processList, Process.BY_ARRIVAL_TIME);
		
	}
	public void simulate()
	{
		
		
		int currentTime=0;
	
		for(int i=0;i<count;i++)
		{
			Process current=processList.get(i);
			if(currentTime<current.getArrivalTime())
				currentTime=current.getArrivalTime();
			current.setStartTime(currentTime);
			current.setEndTime(current.getStartTime()+current.getBurstTime());
			ganntP[j]=current.getProcessId();
			ganntT[j]=currentTime;
			j++;
			currentTime+=current.getBurstTime();
			
		}
		for(int i=0;i<count;i++)
		{	
			Process current=processList.get(i);
			current.setWaitingTime(current.getStartTime()-current.getArrivalTime());
			current.setTurnaroundTime(current.getEndTime()-current.getArrivalTime());
			
			totalWaitingTime+=current.getWaitingTime();
			totalTurnAroundTime+=current.getTurnaroundTime();
		}
		avgWatingTime=(float)totalWaitingTime/count;
		avgTurnaroundTime=(float)totalTurnAroundTime/count;
		
	}
	public void printResult()
	{
		Collections.sort(this.processList, Process.BY_PROCESSID);
		System.out.println("Simulation result of FIFO ");
		System.out.println("PId ArrivalT BurstT Priority StartT   EndT WaitingT TurnAroundT");
		for(Process p : processList)
		{
		System.out.println(p);	
		
		}
		System.out.println("Average Waiting Time of FIFO "+avgWatingTime);
		System.out.println("Average TurnAround Time of FIFO "+avgTurnaroundTime);
		for(int i=0;i<j;i++)
		{
			System.out.println("time "+ganntT[i]+" process "+ganntP[i]);	
			
		}	
		System.out.println();
		}
}
