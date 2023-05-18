#pragma once

#ifndef CSE305_PROJECT_DELTA_STEP_SEQ_H

#define CSE305_PROJECT_DELTA_STEP_SEQ_H

#include "graph.h"

class DeltaStepSequential
{
public:
	DeltaStepSequential(const Graph& graph, const int source);
	~DeltaStepSequential () = default;


    void solve();
    void print_solution();
    void solve_light_heavy();
private:

	void compute_light_and_heavy_edges();
	void find_requests(std::vector<Edge>* light_requests, std::vector<Edge>* heavy_requests);

	void print_light_and_heavy_edges();
	void print_all_buckets() const;
	void print_bucket(size_t bucket_id) const;




	Graph graph_;
	int source_;
	std::vector<double> dist_;
	std::vector<int> pred_;
	std::set<int> bucket_inf_;
	std::vector<std::set<int>> buckets_;
	double delta_;
	std::vector<std::vector<int>> light_edges_;
	std::vector<std::vector<int>> heavy_edges_;
	std::vector<std::vector<double>> adj_matrix_;
	int bucket_counter_ = 0;

};

#endif // !CSE305_PROJECT_DELTA_STEP_SEQ_H
