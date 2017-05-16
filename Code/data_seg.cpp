//sensor 0
//raw data that continuously storing sensor data
std::vector<std::vector<float>> rawDataS0; 
//
std::vector<std::vector<float>> stabilityLst;
//contains sensor data when the foot is on ground
std::vector<std::vector<float>> interestDataS0;
int fyll = 0; 
int start_to_fill = 0; //Enables when the foot is on ground

void print_s(int sensorNr){
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

/*
*This function check whether the sensor data has minor change
*If the neighbors in the elements is over 0.009 which mean
*that the foot is on ground.  
*********/
int isStable(std::vector<std::vector<float>> stabArr){
    float boundary = 0.009; //Threshold for 
    int stabilityCount = 0;
    int pos1 = stabArr.size()-2;
    int pos2 = stabArr.size()-1;

    int i;
    for(i=0;i<3;i++){
        float current = stabArr.at(pos1).at(i);
        float next = stabArr.at(pos2).at(i);

        if (fabs(next - current) > boundary) {
            return 0;
        }
    }
    return 1;
}

/*
*This function checking how 
***/
void handleDataS0(){
    if(stabilityLst.size() > 2){
        if(fyll == 0 && isStable(stabilityLst)){
            fyll = 1;
        }
        else if(fyll) {
            if (!isStable(stabilityLst)) {

                if (stabilityLst.size() > stabThres) {
                    start_to_fill = 1;

                    //Fyller den forrige og naavaerende data
                    int lastElement = stabilityLst.size()-1;
                    int secondLast = stabilityLst.size()-2;
                    interestDataS0.push_back(stabilityLst.at(secondLast));
                    interestDataS0.push_back(stabilityLst.at(lastElement));
                }
                fyll = 0;
                stabilityLst.clear();
            }else if( start_to_fill && stabilityLst.size() >= stabThres  ){
                start_to_fill = 0;
                print_s(0);
            }
        }
        else{
            fyll = 0;
            stabilityLst.clear();
        }
    }
}

//This function obtain sensor data
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