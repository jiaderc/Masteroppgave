//sensor 0
std::vector<std::vector<float>> rawDataS0;
std::vector<std::vector<float>> stabilityLst;
std::vector<std::vector<float>> interestDataS0;
int fyll = 0;
int startFyll = 0;

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
    writeToFile();
}


int isStable(std::vector<std::vector<float>> stabArr){
  //  float boundary = 0.03;
    float boundary = 0.009;
    int stabilityCount = 0;
    int pos1 = stabArr.size()-2;
    int pos2 = stabArr.size()-1;

    if(stabArr.size() < 2){
        std::cout << "Noe er rarrt\n";
    }

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


void handleDataS0(){
    if(stabilityLst.size() > 2){
        if(fyll == 0 && isStable(stabilityLst)){
            fyll = 1;
        }
        else if(fyll) {
            if (!isStable(stabilityLst)) {

                if (stabilityLst.size() > stabThres) {
                    startFyll = 1;

                    //Fyller den forrige og naavaerende data
                    int lastElement = stabilityLst.size()-1;
                    int secondLast = stabilityLst.size()-2;
                    interestDataS0.push_back(stabilityLst.at(secondLast));
                    interestDataS0.push_back(stabilityLst.at(lastElement));
                }
                fyll = 0;
                stabilityLst.clear();
            }else if( startFyll && stabilityLst.size() >= stabThres  ){
                startFyll = 0;
                print_s(0);
            }
        }
        else{
            fyll = 0;
            stabilityLst.clear();
        }
    }
}

void optoforceCallback0(const geometry_msgs::WrenchStamped::ConstPtr& msg)
{
    std::vector<float> tmp;
    tmp.push_back(msg->wrench.force.x);
    tmp.push_back(msg->wrench.force.y);
    tmp.push_back(msg->wrench.force.z);
    rawDataS0.push_back(tmp);

    if(startFyll){
        interestDataS0.push_back(tmp);
    }
    stabilityLst.push_back(tmp);
    handleDataS0();
}