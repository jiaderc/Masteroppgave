//sensor 0
//Raw data that continuously storing sensor data
std::vector<std::vector<float>> rawDataS0; 
//Used to detect whether the foot is on ground or not
std::vector<std::vector<float>> stabilityLst;
//Contains sensor data when the foot is on ground
std::vector<std::vector<float>> interestDataS0;
int fill = 0; 
int start_to_fill = 0; //Enables when the foot is on ground

//This function check whether the stored sensor data does not exceed maxBoundary.
//If the 
void checkDesiredData(int sensorNr)
{
    int maxBoundary = 145;
    if(sensorNr == 0) {
        interestDataS0.resize(interestDataS0.size() - stabilityLst.size());		
        if (interestDataS0.size() < expected_length_data ||
            interestDataS0.size() > maxBoundary) {
            interestDataS0.clear();
            return;
        }       	
	}
    writeToFile(); //A function that write the data to file (not shown in code)
}

int isStable(std::vector<std::vector<float>> stabilityArray)
{
    float boundary = 0.009;
    int stabilityCount = 0;
    int pos1 = stabilityArray.size()-2;
    int pos2 = stabilityArray.size()-1;

    int i;
    for(i=0;i<3;i++){
        float current = stabilityArray.at(pos1).at(i);
        float next = stabilityArray.at(pos2).at(i);

        if (fabs(next - current) > boundary) {
            return 0;
        }
    }
    return 1;
}

void handleDataS0()
{
    if(stabilityLst.size() > 2){
        if(fill == 0 && isStable(stabilityLst)){
            fill = 1;
        }
        else if(fill) {
            if (!isStable(stabilityLst)) {

                if (stabilityLst.size() > stabThres) {
                    start_to_fill = 1;
                    int lastElement = stabilityLst.size()-1;
                    int secondLast = stabilityLst.size()-2;
                    interestDataS0.push_back(stabilityLst.at(secondLast));
                    interestDataS0.push_back(stabilityLst.at(lastElement));
                }
                fill = 0;
                stabilityLst.clear();
            }else if( start_to_fill && stabilityLst.size() >= stabThres  ){
                start_to_fill = 0;
                print_s(0);
            }
        }
        else{
            fill = 0;
            stabilityLst.clear();
        }
    }
}

//This function obtains sensor data
void optoforceCallback0(const geometry_msgs::WrenchStamped::ConstPtr& msg)
{
    std::vector<float> tmp;
	//Storing sensor data
    tmp.push_back(msg->wrench.force.x);
    tmp.push_back(msg->wrench.force.y);
    tmp.push_back(msg->wrench.force.z);
    rawDataS0.push_back(tmp);

    if(start_to_fill){
        interestDataS0.push_back(tmp);
    }
    stabilityLst.push_back(tmp);
    handleDataS0();
}