#ifndef CLASSIFICATION_DATA_H
#define CLASSIFICATION_DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>

//#include "DecisionTree.h"

using namespace std;

template <typename T> class CPattern
{
public:
	vector<T> m_pattern_data;
	int m_num_data_points; // dimensionality of this patter;
	int m_classification;
};

template <typename T> class CClassificationData
{
	//friend class CDecisionTree<typename T>;
	template<class T> friend class CDecisionTree;
	template<class T> friend class CKNearestNeighbourClassifier;
	//template<class T> 
protected:
	vector<CPattern<T> > m_data_sets;
	int m_num_data_sets;
public:
	void ReadCSVFile(string filename,int num_data_sets,int values_per_data_set,bool classification_data);
	void PrintPatterns(int index_start,int index_end);
	CPattern<T> ReturnPattern(int index)
	{
		return m_data_sets[index];
	}
};

template <typename T> void CClassificationData<T>::PrintPatterns(int index_start,int index_end)
{
	for(int i=index_start;i<index_end;i++)
	{
		for(int j=0;j<m_data_sets[i].m_num_data_points;j++)
			cout << m_data_sets[i].m_pattern_data[j] << " ";
		cout << "classification: " << m_data_sets[i].m_classification << endl;
	}
}

template <typename T> void CClassificationData<T>::ReadCSVFile(string filename,int num_data_sets,int values_per_data_set,bool classification_data)
{
	ifstream is(filename,ios::out);
	is.seekg(0,is.end);
	int length=is.tellg();
	is.seekg(0,is.beg);

	char* buffer = new char[length+1];
	cout << "length of file: " << length << endl;
	is.read(buffer,length);
	cout << "file read" << endl;
	
	int line_counter=0;	
	char *string = &buffer[0];
	char *token = strchr(string,'\n');

	// skip first line !!
	*token++='\0';
	string=token;
	token = strchr(string,'\n');
	// done skipping first line

	char* token2;
	char* pEnd;
	int classification,value;
	int value_counter=0;

	int max_value_index;
	if(classification_data==true)
		max_value_index = values_per_data_set;
	else
		max_value_index = values_per_data_set-1;

	while((token!=NULL)&&(line_counter<num_data_sets))
	{
		*token++='\0';
		
		CPattern<int> curr_pattern;
		curr_pattern.m_num_data_points = values_per_data_set;
		if(classification_data==true)
		{
			token2 = strtok(string,",");
			classification = strtol(token2,&pEnd,10);
			curr_pattern.m_classification=classification;
		}
		else
		{
			token2 = strtok(string,",");
			value=strtol(token2,&pEnd,10);
			curr_pattern.m_pattern_data.push_back(value);
			curr_pattern.m_classification=-1;
		}
		value_counter=0;
		while((token2!=NULL)&&(value_counter<max_value_index))
		{
			
			token2=strtok(NULL,",");
			
			value=strtol(token2,&pEnd,10);
			curr_pattern.m_pattern_data.push_back(value);
			value_counter++;
		}
		this->m_data_sets.push_back(curr_pattern);

		if(line_counter%1000==0)
			cout << line_counter << endl;
		string=token;
		token=strchr(string,'\n');
		line_counter++;
	}
	this->m_num_data_sets=num_data_sets;

	cout << "data read" << endl;
	delete[] buffer;
}

#endif
