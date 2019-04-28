
#include "lsystem.h"
#include <stack>
#include <memory>
#include <iostream>

/*
Provided utilities:

- Dice class (utils/misc.h)
	Produces random values uniformly distributed between 0 and 1
	Example:
		Dice d;
		double random_val = d.roll();

- write_string_to_file (utils/misc.h)
	Write string data into a text file.
	Example:
		write_string_to_file("ala ma kota!", "ala.txt");
*/

std::string LindenmayerSystemDeterministic::expandSymbol(unsigned char const& sym) {
	auto map = this->rules;
	auto charExpansion = map.find(sym); // first is the key, second is the value

	// if no rule found, return char (for '+', '-', etc) otherwise return rule
	return charExpansion == map.end()? std::string(1, sym) : charExpansion->second;
}

std::string LindenmayerSystem::expandOnce(std::string const& symbol_sequence) {
	std::string result = "";
	for(size_t i=0; i<symbol_sequence.size(); i++){
		result += expandSymbol(symbol_sequence[i]);
	}
	return result;
}

std::string LindenmayerSystem::expand(std::string const& initial, uint32_t num_iters) {
	std::string result = initial;
	for(size_t i=0; i<num_iters; i++){
		result = expandOnce(result);
	}
	return result;
}

#define ANGLE this->rotation_angle_deg*M_PI/180 // in radians

void createNewBranch(std::stack<std::vector<Segment>>* branches, vec2 initialPosition, vec2 initialDirection){
	std::vector<Segment> newBranch;
	newBranch.push_back(Segment(initialPosition, initialDirection));
	branches->push(newBranch);
}

void resolveBranch(std::stack<std::vector<Segment>>* branches, std::vector<Segment>* segmentsToDraw){
	std::vector<Segment> branchToResolve = branches->top();
	
	// copy all segments except the first one which is the initial pair (position, direction) for the branch
	for(size_t i=1; i<branchToResolve.size(); i++){
		segmentsToDraw->push_back(branchToResolve[i]);
	}

	// remove and delete elements of resolved branch
	branches->pop();
}

void changeAngle(double angle, std::vector<Segment>* currentBranch){
	vec2 position = currentBranch->begin()->first;
	vec2 direction = currentBranch->begin()->second;
	double x = direction.x;
	double y = direction.y;
	double sin_ = std::sin(angle);
	double cos_ = std::cos(angle);

	// Rotating a 2d vector by applying the Rotation matrix: https://en.wikipedia.org/wiki/Rotation_matrix
	vec2 newDirection = vec2(x*cos_-y*sin_, x*sin_ + y*cos_);

	// update the info for the branch
	currentBranch->at(0) = Segment(position, newDirection);
}

void addSegment(vec2 origin, vec2 destination, std::vector<Segment>* currentBranch){
	vec2 direction = currentBranch->begin()->second;

	// Add segment to branch
	currentBranch->push_back(Segment(origin, destination));

	// Update position vector
	currentBranch->at(0) = Segment(destination, direction);
}


std::vector<Segment> LindenmayerSystem::draw(std::string const& symbols) {
	std::vector<Segment> segmentsToDraw;
	std::stack<std::vector<Segment>> branches;
	// the first element of each branch is the pair (position, direction) after that, we have the segments that compose it

	// create first branch with intitial position(0,0) and initial direction(0, 1)
	createNewBranch(&branches, vec2(0,0), vec2(0,1));

	for(size_t i=0; i<symbols.size(); i++){
		std::vector<Segment>* currentBranch = &(branches.top());
		vec2 position = currentBranch->begin()->first;
		vec2 direction = currentBranch->begin()->second;

		switch(symbols[i]){
			case '+': // rotate direction by δ counter-clockwise
				changeAngle(ANGLE, currentBranch);
				break;
			case '-': // rotate direction by δ clockwise
				changeAngle(-ANGLE, currentBranch);
				break;
			case '[': // new branch, save (position, direction)
				createNewBranch(&branches, position, direction);
				break;
			case ']': // end of branch, restore previous branch's state (position, direction)
				resolveBranch(&branches, &segmentsToDraw);
				break;
			case 'F': // "for F or any other symbol"
			default: // draw line of length 1 (according to direction)
				addSegment(position, position + direction, currentBranch);
				break;
		}
	}

	// resolve initial branch
	resolveBranch(&branches, &segmentsToDraw);

	return segmentsToDraw;
}

std::string LindenmayerSystemStochastic::expandSymbol(unsigned char const& sym) {
	auto map = this->rules;
	auto rulesForChar = map.find(sym); // first is the key [char], second is the value [vector<StochasticRule>]


	double randomNb = dice.roll();
	double probability = 0;

	for(size_t i=0; rulesForChar != map.end() && i<rulesForChar->second.size(); i++){ // if rulesForChar == map.end() -> do not loop because there is nothing to loop on
		auto rule = rulesForChar->second[i];
		probability+= rule.probability;
		if(randomNb <= probability)
			return rule.expansion;
	}

	return std::string(1, sym); // no rule found, return char itself (eg for '+', '-', etc)
}

void LindenmayerSystemDeterministic::addRuleDeterministic(unsigned char sym, std::string const& expansion) {
	rules[sym] = expansion;
}

void LindenmayerSystemStochastic::addRuleStochastic(unsigned char sym, std::vector<StochasticRule> expansions_with_ps) {
	rules[sym] = expansions_with_ps;
}
