// File:     proj2.cpp
//
// Group A7
//
//
//
// project sourse file

#include "proj2.hpp"
#define MAX_TIME 999



//Shortest job first Algorithm
void shortestJob(list<pcb>& listOfPCB)
{
    double number_of_processes = listOfPCB.size();
	vector<int> replaceVector;
	list<pcb>::iterator pcbIterator = listOfPCB.begin();
	//Universal time unit counter
	int timer = 0;

	while (!listOfPCB.empty())
	{
		int temp = 0;
        int tempPriority = 0;
		int shortest = MAX_TIME;
		int tempTarq = 0;
		int firstTarq = MAX_TIME;
        int highestPriority = MAX_TIME;
		listOfPCB.begin();
		//Checks each of the processes for the shortest CPU bursts
		for (int i = 0; i < listOfPCB.size(); i++)
		{
			if (pcbIterator->getTarq() <= timer)
			{
				temp = pcbIterator->getCPU_bursts().front();
                tempPriority = pcbIterator->getPriority();
				if (temp <= shortest)
                {
                    shortest = temp;
                    highestPriority = tempPriority;
                    //If there is a tie in who runs shorter then tie break it with priority comparison
                    if (temp == shortest)
                    {
                        if (tempPriority < highestPriority)
                            highestPriority = tempPriority;
                    }
                }
			}
			pcbIterator++;
		}
		pcbIterator = listOfPCB.begin();

		//This loop deletes the shortest CPU run so that it doesn't exist in the next check as well as prints the gant chart
		if (shortest != MAX_TIME)
		{
			for (int i = 0; i < listOfPCB.size(); i++)
			{
				if ((pcbIterator->getCPU_bursts().front() == shortest) && (pcbIterator->getPriority() == highestPriority))
				{
                    int waiting = timer - pcbIterator->tarq_;
                    //waiting = timer;

//                    cout << endl << endl;
                    
					//Prints out the Gantt chart with each PID printed out representing a time unit
					for (int k = 0; k < shortest; k++)
					{
						cout << pcbIterator->getPid();
						//Each time CPU bursts is a time unit
						timer++;
					}
                    
                    int turnaround = timer;
                    
                    //cout << "\nThe waiting time for process " << pcbIterator->getPid() << " this time is: " << waiting << endl;
                    
                   // cout << "The turnaround time for process " << pcbIterator->getPid() << " this time is: " << turnaround << endl << endl;
                    
                    //addMetric(pcbIterator->tarq_, timer);
                    waiting_times.push_front(waiting);
                    completion_times.push_front(turnaround);
                    
                    //Change the tarq to compensate for IO bursts
                    pcbIterator->setTarq(timer+pcbIterator->getIO_bursts().front());
                    
                    //Get rid of finished CPU bursts
					replaceVector = pcbIterator->getCPU_bursts();
					replaceVector.erase(replaceVector.begin());
					pcbIterator->setCPU_bursts(replaceVector);
                    
                    //Get rid of IO bursts
                    replaceVector = pcbIterator->getIO_bursts();
                    replaceVector.erase(replaceVector.begin());
                    pcbIterator->setIO_bursts(replaceVector);
				}
				if (pcbIterator->getCPU_bursts().empty())
				{
					listOfPCB.erase(pcbIterator);
					pcbIterator++;
				}
				else
					pcbIterator++;
			}
		
			pcbIterator = listOfPCB.begin();
		}
		else
		{
			//Else no processors arrive, then proceed one time unit
            cout << " ";
			timer++;
		}
	}
    cout << endl << endl;
    printMetrics(completion_times, waiting_times, number_of_processes);
}


//Shortest job first with pre-emption Algorithm
void shortestJobWithPreemption(list<pcb>& listOfPCB)
{
	vector<int> replaceCPUVector;
    vector<int> replaceIOVector;
	list<pcb>::iterator pcbIterator = listOfPCB.begin();
	//Universal time unit counter
	int timer = 0;
    int oldPid = MAX_TIME;
    double number_of_processes = listOfPCB.size();
    int waiting = 0;
    int turnaround = 0;
    
	while (!listOfPCB.empty())
	{
		int temp = 0;
        int tempPriority = 0;
		int shortest = MAX_TIME;
        int highestPriority = MAX_TIME;
		listOfPCB.begin();
		//Checks each of the processes for the shortest CPU bursts
		for (int i = 0; i < listOfPCB.size(); i++)
		{
			if ((pcbIterator->getTarq() <= timer))
			{
				temp = pcbIterator->getCPU_bursts().front();
                tempPriority = pcbIterator->getPriority();
				if (temp <= shortest && temp != 0)
                {
                    shortest = temp;
                    highestPriority = tempPriority;
                    //If there is a tie in who runs shorter then tie break it with priority comparison
                    if (temp == shortest)
                    {
                        if (tempPriority < highestPriority)
                            highestPriority = tempPriority;
                    }
                }
			}
			pcbIterator++;
		}
		pcbIterator = listOfPCB.begin();
        
		//This loop deletes the shortest CPU run so that it doesn't exist in the next check as well as prints the gant chart
		if (shortest != MAX_TIME)
		{
			for (int i = 0; i < listOfPCB.size(); i++)
			{
                
                if (pcbIterator->cpu_bursts_[0] == 0)
                {
                    pcbIterator->tarq_ = timer + pcbIterator->io_bursts_[0] - 1;
                    if(pcbIterator->io_bursts_[0] > 0)
                    {
                        pcbIterator->io_bursts_[0] = pcbIterator->io_bursts_[0] - 1;
                    }
                    else if (pcbIterator->io_bursts_[0] == 0)
                    {
                        pcbIterator->io_bursts_.erase(pcbIterator->io_bursts_.begin());
                        pcbIterator->cpu_bursts_.erase(pcbIterator->cpu_bursts_.begin());
                    }
                }
            
				else if ((pcbIterator->getCPU_bursts().front() == shortest) && (pcbIterator->getPriority() == highestPriority))
				{
                    turnaround = timer + 1;
                    
                    if (pcbIterator->cpu_bursts_[0] > 0)
                    {
                        
                        if (pcbIterator->pid_ != oldPid)
                        {
                            oldPid = pcbIterator->pid_;
                            waiting = timer - pcbIterator->tarq_;
                            //cout << "\nThe waiting time for process " << pcbIterator->getPid() << " this time is: " << waiting << endl;
                            
                            //cout << "The turnaround time for process " << pcbIterator->getPid() << " this time is: " << turnaround << endl;
                            
                            addMetric(pcbIterator->tarq_, timer);
                        }
                        
                        //Prints out the Gantt chart with each PID printed out representing a time unit
                        //cout << "|" << pcbIterator->getPid() << "|" << ' ';
                        cout << pcbIterator->getPid();
                        cout.flush();
                        
                        //Each time CPU bursts is a time unit
                        timer++;

                    }
                    
                    //Get rid of finished CPU bursts
                    if (pcbIterator->cpu_bursts_[0] > 0)
                    {
                        pcbIterator->cpu_bursts_[0] = pcbIterator->cpu_bursts_[0] - 1;
                    }
                    else if (pcbIterator->cpu_bursts_[0] == 0)
                    {
                        pcbIterator->tarq_ = timer + pcbIterator->io_bursts_[0] - 1;
                        if(pcbIterator->io_bursts_[0] > 0)
                        {
                            pcbIterator->io_bursts_[0] = pcbIterator->io_bursts_[0] - 1;
                        }
                        else if (pcbIterator->io_bursts_[0] == 0)
                        {
                            pcbIterator->io_bursts_.erase(pcbIterator->io_bursts_.begin());
                            pcbIterator->cpu_bursts_.erase(pcbIterator->cpu_bursts_.begin());
                        }
                    }
                    
                }
                
				if (pcbIterator->getCPU_bursts().empty())
				{
					listOfPCB.erase(pcbIterator);
					pcbIterator++;
				}
				else
					pcbIterator++;
			}
            
			//Test print out string, will be deleted
			//cout << "The shortest currently is: " << shortest << endl;
			//cout << "Current time is: " << timer << endl << endl;
			pcbIterator = listOfPCB.begin();
		}
		else
        //No processes ready, most likely all in queue
		{
            for (int i = 0; i < listOfPCB.size(); i++)
			{
                if (pcbIterator->cpu_bursts_[0] == 0)
                {
                    pcbIterator->tarq_ = timer + pcbIterator->io_bursts_[0] - 1;
                    if(pcbIterator->io_bursts_[0] > 0)
                    {
                        pcbIterator->io_bursts_[0] = pcbIterator->io_bursts_[0] - 1;
                    }
                    else if (pcbIterator->io_bursts_[0] == 0)
                    {
                        pcbIterator->io_bursts_.erase(pcbIterator->io_bursts_.begin());
                        pcbIterator->cpu_bursts_.erase(pcbIterator->cpu_bursts_.begin());
                    }
					
                }
				
				if (pcbIterator->getCPU_bursts().empty())
				{
					listOfPCB.erase(pcbIterator);
					pcbIterator++;
				}
				else
					pcbIterator++;
            }
			//Else no processors arrive, then proceed one time unit
            cout << " ";
			timer++;
			pcbIterator = listOfPCB.begin();
		}
	}
    cout << "\n \n";
    printMetrics(completion_times, waiting_times, number_of_processes);
}


double previousBurstCalc(int t_n, int tau_n, double alpha)
{
	double result;
    
	result = (alpha*t_n) + ((1 - alpha)*tau_n);
	return result;
}

//Shortest Previous Burst Algorithm
void shortestPreviousBurst(list<pcb>& listOfPCB, double weightCoefficient)
{
    double number_of_processes = listOfPCB.size();
    //Universal time counter
    int timer = 0;
    list<pcb>::iterator pcbIterator = listOfPCB.begin();

	vector<int> replaceVector;

	//Vector that holds previous bursts or (t_n in the lecture slides)
    vector<int> prevBurst;

	//Vector that holds the previous average (tau_n in the lecture slides)
    vector<int> burstAverages;

	//Checks weightCoefficient is valid
	if (weightCoefficient > 1)
		cout << "Error: Weight coefficient (alpha) must be smaller than or equal 1" << endl;

	else
	{
		//Fill all vectors needed with 0
		for (int i = 0; i < listOfPCB.size(); i++)
		{
			burstAverages.push_back(0);
			prevBurst.push_back(0);
		}

		while (!listOfPCB.empty())
		{
			double temp = 0;
			double shortest = MAX_TIME;
            int tempPriority = 0;
            int highestPriority = MAX_TIME;
		    //Collects next CPU bursts
			 for (int i = 0; i < listOfPCB.size(); i++)
			 {
				if (pcbIterator->getTarq() <= timer)
				{
				    temp = pcbIterator->getCPU_bursts().front();
					prevBurst[i] = temp;
				}
				else
				{
					temp = 0;
					prevBurst[i] = temp;
				}
				pcbIterator++;
			}

			pcbIterator = listOfPCB.begin();

			//Compares for the shortest
			for (int i = 0; i < listOfPCB.size(); i++)
			{
				if (pcbIterator->getTarq() <= timer)
				{
					temp = previousBurstCalc(prevBurst[i], burstAverages[i], weightCoefficient);
					tempPriority = pcbIterator->getPriority();
                    if (temp <= shortest)
                    {
                        shortest = temp;
                        highestPriority = tempPriority;
                        //If there is a tie in who runs shorter then tie break with priority comparison
                        if (temp == shortest)
                        {
                            if (tempPriority < highestPriority)
                                highestPriority = tempPriority;
                        }
                    }

				}
				pcbIterator++;
			}

			pcbIterator = listOfPCB.begin();

			if (shortest != MAX_TIME)
			{
				for (int i = 0; i < listOfPCB.size(); i++)
				{
                    int turnaround = 0;
                    int waiting = 0;
					double temp2 = previousBurstCalc(pcbIterator->getCPU_bursts().front(), burstAverages[i],weightCoefficient);
					if ((temp2 == shortest) && (pcbIterator->getPriority() == highestPriority))
					{
						burstAverages[i] = int(temp2);
						//Prints out the Gantt chart with each PID printed out representing a time unit
                       
                        waiting = timer - pcbIterator->tarq_;
                        //waiting = timer;
                        
                        
						for (int k = 0; k < pcbIterator->getCPU_bursts().front(); k++)
						{
							cout << pcbIterator->getPid();
							//Each time CPU bursts is a time unit
							timer++;
						}
                        
                        turnaround = timer;
                        
                        //cout << "\nThe waiting time for process " << pcbIterator->getPid() << " this time is: " << waiting << endl;
                        
                        //cout << "The turnaround time for process " << pcbIterator->getPid() << " this time is: " << turnaround << endl << endl;
                        
                        //addMetric(pcbIterator->tarq_, timer);
                        waiting_times.push_front(waiting);
                        completion_times.push_front(turnaround);
                       
                        //Change the tarq to compensate for IO bursts
                        pcbIterator->setTarq(timer+pcbIterator->getIO_bursts().front());
                        
						//Deletes the first cpu bursts in the vector that was already used above and pushes everything else forward
						replaceVector = pcbIterator->getCPU_bursts();
						replaceVector.erase(replaceVector.begin());
						pcbIterator->setCPU_bursts(replaceVector);
                        
                        //Get rid of first IO bursts
                        replaceVector = pcbIterator->getIO_bursts();
                        replaceVector.erase(replaceVector.begin());
                        pcbIterator->setIO_bursts(replaceVector);
					}
					
					//Checks if there are still anymore cpu bursts in the PCB; if there aren't, delete it
					if (pcbIterator->getCPU_bursts().empty())
					{
						listOfPCB.erase(pcbIterator);
						pcbIterator++;
					}
					else
						pcbIterator++;
				}
		
				pcbIterator = listOfPCB.begin();
			}
            //No processes in CPU
			else
			{
                cout << " ";
				timer++;
			}
		}
        cout << endl << endl;
        printMetrics(completion_times, waiting_times, number_of_processes);
	}
    
}

