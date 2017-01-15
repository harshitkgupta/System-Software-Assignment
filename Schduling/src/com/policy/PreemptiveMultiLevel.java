package com.policy;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class PreemptiveMultiLevel {

	List<Process> processList;
	
	private int count;
	private int timeQuantum = 1;
	int j=0;
	private int ganntP[];
	private int ganntT[];
	private int totalWaitingTime = 0;
	private int totalTurnAroundTime = 0;
	private float avgPriority = 0;
	private float avgWatingTime = 0;
	private float avgTurnaroundTime = 0;

	PreemptiveMultiLevel(List<Process> processList) {
		count = processList.size();
		ganntT=new int[200];
		ganntP=new int[200];
		this.processList = new ArrayList<Process>();

		for (Process p : processList) {

			avgPriority += p.getPriority();
			this.processList.add(new Process(p.getProcessId(), p
					.getArrivalTime(), p.getBurstTime(), p.getPriority()));

		}
		avgPriority /= (float) count;
		Collections.sort(processList, Process.BY_ARRIVAL_TIME);

	}

	public void simulate() {

		int currentTime = 0;
		int remainingProcess = count;

		while (remainingProcess > 0) {
			float fmin = 5;
			int barr = 100000;
			int findex = -1;
			int bindex = -1;
			Process current = null;

			for (int i = 0; i < count; i++) {
				current = processList.get(i);

				if (current.getPriority() < fmin
						&& current.getRemainingTime() > 0
						&& current.getArrivalTime() <= currentTime) {
					findex = i;
					fmin = (float) current.getPriority();
				}
			}
			if (findex == -1) {
				for (int i = 0; i < count; i++) {
					current = processList.get(i);

					if (current.getArrivalTime() < barr
							&& current.getRemainingTime() > 0
							&& current.getArrivalTime() <= currentTime) {
						bindex = i;
						barr = current.getArrivalTime();

					}
				}
				if (bindex == -1) {
					currentTime++;
					continue;
				}
			}
			if (findex != -1) {
				current = processList.get(findex);
				if (current.getStartTime() == -1) {
					current.setStartTime(currentTime);
				}
				ganntP[j]=current.getProcessId();
				ganntT[j]=currentTime;
				j++;
				current.setRemainingTime(current.getRemainingTime() - 1);
				currentTime++;
				if (current.getRemainingTime() == 0) {
					current.setEndTime(currentTime);

					remainingProcess--;
				}
			}
			if (bindex != -1) {
				current = processList.get(bindex);
				if (current.getStartTime() == -1)
					current.setStartTime(currentTime);
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
				.println("Simulation result of Preemptive Multilevel Schduling ");
		System.out
				.println("PId ArrivalT BurstT Priority StartT   EndT WaitingT TurnAroundT");
		for (Process p : processList) {
			System.out.println(p);

		}
		System.out.println("Average Waiting Time of Preemptive Multilevel "
				+ avgWatingTime);
		System.out.println("Average TurnAround Time of Preemptive Multilevel "
				+ avgTurnaroundTime);
		for(int i=0;i<j;i++)
		{
			System.out.println("time "+ganntT[i]+" process "+ganntP[i]);	
			
		}
		System.out.println();
	}
}
