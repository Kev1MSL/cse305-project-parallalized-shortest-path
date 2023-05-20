#pragma once

#ifndef CSE305_PROJECT_DELTA_STEP_SEQ_H

#define CSE305_PROJECT_DELTA_STEP_SEQ_H

#include "graph.h"

class DeltaStepSequential
{
public:
	DeltaStepSequential(const Graph& graph, const int source, bool is_verbose = false);
	~DeltaStepSequential () = default;


    void solve();
    void print_solution();
    void solve_light_heavy();
	[[nodiscard]] std::vector <double> get_dist() const { return dist_; }
	[[nodiscard]] double get_dist(const int source) const { return get_dist()[source]; }
private:

	void compute_light_and_heavy_edges();
	void find_bucket_requests(const std::set<int>& bucket, std::vector<Edge>* light_requests, std::vector<Edge>* heavy_requests);
	void relax(Edge selected_edge);
	void resolve_requests(std::vector<Edge>* requests);


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
	bool is_verbose_;

};

#endif // !CSE305_PROJECT_DELTA_STEP_SEQ_H
