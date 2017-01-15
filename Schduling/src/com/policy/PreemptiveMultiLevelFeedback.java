package com.policy;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class PreemptiveMultiLevelFeedback {

	List<Process> processList;
	List<Process> fprocessList;
	List<Process> bprocessList;
	int j=0;
	private int ganntP[];
	private int ganntT[];
	private int count;
	private int timeQuantum = 1;

	private int totalWaitingTime = 0;
	private int totalTurnAroundTime = 0;
	
	private float avgWatingTime = 0;
	private float avgTurnaroundTime = 0;

	PreemptiveMultiLevelFeedback(List<Process> processList) {
		count = processList.size();
		ganntT=new int[200];
		ganntP=new int[200];
		this.processList = new ArrayList<Process>();

		for (Process p : processList) {

			
			this.processList.add(new Process(p.getProcessId(), p
					.getArrivalTime(), p.getBurstTime(), 0));

		}
		
		Collections.sort(processList, Process.BY_ARRIVAL_TIME);

	}

	public void simulate() {

		int currentTime = 0;
		int remainingProcess = count;

		while (remainingProcess > 0) 
		{
			float fmin1 = 3;
			float fmin2=6;
			int barr = 100000;
			int aindex = -1;
			int bindex = -1;
			int cindex = -1;
			Process current = null;

			for (int i = 0; i < count; i++)
			{
				current = processList.get(i);

				if (current.getPriority() < fmin1
						&& current.getRemainingTime() > 0
						&& current.getArrivalTime() <= currentTime) 
				{
					aindex = i;
					fmin1 = (float) current.getPriority();
				}
			}
			if (aindex == -1)
			{
				for (int i = 0; i < count; i++)
				{
					current = processList.get(i);

					if (current.getPriority()< fmin2
							&& current.getRemainingTime() > 0
							&& current.getArrivalTime() <= currentTime)
					{
						bindex = i;
						fmin2 = (float) current.getPriority();

					}
			    }
				if (bindex == -1) 
				{
					for (int i = 0; i < count; i++) 
					{
						current = processList.get(i);

						if (current.getArrivalTime() < barr
								&& current.getRemainingTime() > 0
								&& current.getArrivalTime() <= currentTime) 
						{
							cindex = i;
							barr = current.getArrivalTime();

						}
				}
				if(cindex==-1)	
				{
					currentTime++;
					continue;
				}
			}
		}
			timeQuantum=2;
			if (aindex != -1) {
				current = processList.get(aindex);
				current.setPriority(current.getPriority()+1);
				if (current.getStartTime() == -1) {
					current.setStartTime(currentTime);
				}
				ganntP[j]=current.getProcessId();
				ganntT[j]=currentTime;
				j++;
				if (current.getRemainingTime() <= timeQuantum
						&& current.getEndTime() == -1) {
					current.setEndTime(currentTime + current.getRemainingTime());
					currentTime += current.getRemainingTime();
					current.setRemainingTime(0);
					remainingProcess--;
				} else if (current.getRemainingTime() > timeQuantum) {
					currentTime += timeQuantum;
					current.setRemainingTime(current.getRemainingTime()
							- timeQuantum);
			}
			}
			timeQuantum=4;
			if (bindex != -1) {
				current = processList.get(bindex);
				current.setPriority(current.getPriority()+1);
				ganntP[j]=current.getProcessId();
				ganntT[j]=currentTime;
				j++;
				if (current.getRemainingTime() <= timeQuantum
						&& current.getEndTime() == -1) {
					current.setEndTime(currentTime + current.getRemainingTime());
					currentTime += current.getRemainingTime();
					current.setRemainingTime(0);
					remainingProcess--;
				} else if (current.getRemainingTime() > timeQuantum) {
					currentTime += timeQuantum;
					current.setRemainingTime(current.getRemainingTime()
							- timeQuantum);
			}
				timeQuantum=1;
			if (cindex != -1) {
				current = processList.get(cindex);
				ganntP[j]=current.getProcessId();
				ganntT[j]=currentTime;
				j++;
				if (current.getRemainingTime() <= timeQuantum
						&& current.getEndTime() == -1) {
					current.setEndTime(currentTime + current.getRemainingTime());
					currentTime += current.getRemainingTime();
					current.setRemainingTime(0);
					remainingProcess--;
				} else if (current.getRemainingTime() > timeQuantum) {
					currentTime += timeQuantum;
					current.setRemainingTime(current.getRemainingTime()
							- timeQuantum);
				}

	

			}
		}
	}
		for (int i = 0; i < count; i++) {
			Process current = processList.get(i);
			current.setWaitingTime(current.getEndTime()
					- current.getBurstTime() - current.getArrivalTime());
			current.setTurnaroundTime(current.getEndTime()
					- current.getArrivalTime());

			totalWaitingTime += current.getWaitingTime();
			totalTurnAroundTime += current.getTurnaroundTime();
		}

		avgWatingTime = (float) totalWaitingTime / count;
		avgTurnaroundTime = (float) totalTurnAroundTime / count;

	}

	public void printResult() {

		Collections.sort(this.processList, Process.BY_PROCESSID);
		System.out
				.println("Simulation result of Preemptive MultilevelFeedback Schduling ");
		System.out
				.println("PId ArrivalT BurstT Priority StartT   EndT WaitingT TurnAroundT");
		for (Process p : processList) {
			System.out.println(p);

		}
		System.out.println("Average Waiting Time of Preemptive Multilevel Feedback "
				+ avgWatingTime);
		System.out.println("Average TurnAround Time of Preemptive Multilevel Feedback  "
				+ avgTurnaroundTime);
		for(int i=0;i<j;i++)
		{
			System.out.println("time "+ganntT[i]+" process "+ganntP[i]);	
			
		}
		System.out.println();
	}
}
