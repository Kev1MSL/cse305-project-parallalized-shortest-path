#ifndef CSE305_PROJECT_DELTA_STEP_PARALLEL_H

#define CSE305_PROJECT_DELTA_STEP_PARALLEL_H

#include "graph.h"
#include <thread>
#include <mutex>

class DeltaStepParallel
{
public:
	DeltaStepParallel(const Graph& graph, const int source, const int thread_number, bool is_verbose = false);
	~DeltaStepParallel() = default;
	void solve();
	void print_solution();
	[[nodiscard]] std::vector <double> get_dist() const { return dist_; }
	[[nodiscard]] double get_dist(const int source) const { return get_dist()[source]; }

private:
	// Same as sequential
	void compute_light_and_heavy_edges();
	void relax(Edge selected_edge);

	// BUG: This function does not seems to be thread safe, getting violation reading location, seems to be a problem with when emplacing into the vector
	void find_bucket_requests(
		std::vector<std::vector<Edge>>* light_requests,
		std::vector<std::vector<Edge>>* heavy_requests,
		std::set<int>::const_iterator begin, 
		const std::set<int>::const_iterator& end,
		int &nb_light_requests, int &nb_heavy_requests
	);
	void find_bucket_requests_seq(const std::set<int>& bucket, std::vector<std::vector<Edge>>* light_requests, std::vector<std::vector<Edge>>* heavy_requests);
	void resolve_requests(
		const std::vector<Edge>* requests,
		const size_t begin,
		const size_t end
	);


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
	
	int bucket_counter_ = 0;
	bool is_verbose_ = false;

	int thread_number_;
	std::mutex light_request_mutex_;
	std::mutex heavy_request_mutex_;
	std::mutex relax_bucket_mutex_;
	std::mutex erase_bucket_mutex_;
	std::vector<std::mutex> bucket_mutexes_;
};

#endif // !CSE305_PROJECT_DELTA_STEP_PARALLEL_H