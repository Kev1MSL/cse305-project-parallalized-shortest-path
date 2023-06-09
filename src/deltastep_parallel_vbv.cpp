#include "deltastep_parallel.h"
#include <mutex>

DeltaStepParallel::DeltaStepParallel(const Graph& graph, const int source, const int thread_number, bool is_verbose)
{
	graph_ = graph;
	source_ = source;
	
	delta_ = (static_cast<double>(graph_.getMaxDegree()) + 1);
	printf("Delta: %f\n", delta_);
	is_verbose_ = is_verbose;
	thread_number_ = thread_number;

	// Initialize the graph
	graph_.computeDegrees();
	graph_.createAdjList();
	graph_.computeMaxDegree();

	// Initialize the algorithm
	dist_ = std::vector<double>(graph_.getGraphNbVertices(), std::numeric_limits<double>::infinity());
	pred_ = std::vector<int>(graph_.getGraphNbVertices(), -1);
	light_edges_ = std::vector<std::vector<int>>(graph_.getGraphNbVertices(), std::vector<int>());
	heavy_edges_ = std::vector<std::vector<int>>(graph_.getGraphNbVertices(), std::vector<int>());

	this->compute_light_and_heavy_edges();
	const int bucket_size = static_cast<int> (graph_.getGraphNbVertices() / delta_) + 1;
	buckets_.resize(bucket_size);
	//create an array of bucket modification locks
	// std::vector<std::mutex> bucket_modification_mutexes_ = std::vector<std::mutex>(bucket_size);
	if (is_verbose_)
		std::cout << "Bucket size: " << bucket_size << std::endl;
	for (int i = 0; i < graph_.getGraphNbVertices(); i++)
	{
		if (i != source_)
			buckets_[bucket_size - 1].insert(i);
	}
	// Set the initial bucket
	buckets_[0].insert(source_);
	dist_[source_] = 0;
	pred_[source_] = source_;
	if (is_verbose_)
	{
		// Display information before the algorithm starts
		this->print_light_and_heavy_edges();
		graph_.print_graph_info();
		this->print_all_buckets();
	}
}

void DeltaStepParallel::compute_light_and_heavy_edges()
{
	for (const Edge& edge : graph_.getEdges())
	{
		if (edge.get_weight() <= delta_)
		{
			light_edges_[edge.get_from()].push_back(edge.get_to());
		}
		else
		{
			heavy_edges_[edge.get_from()].push_back(edge.get_to());
		}
	}
}

void DeltaStepParallel::relax(Edge selected_edge)
{
	const int from_vertex = selected_edge.get_from();
	const int to_vertex = selected_edge.get_to();
	const double edge_weight = selected_edge.get_weight();

	// Lock for relax bucket, because the buckets_ list might be modified (especially the size) and dist_[to_vertex] might be tempered with
	//std::lock_guard<std::mutex> lock(relax_bucket_mutex_);
	const double tentative_dist = dist_[from_vertex] + edge_weight;
	if (tentative_dist < dist_[to_vertex]) {

		

		const int i = static_cast<int> (std::floor(dist_[to_vertex] / delta_));
		const int j = static_cast<int> (std::floor(tentative_dist / delta_));
		if (i < buckets_.size() && i >= 0)
		{
			std::lock_guard<std::mutex> lock(bucket_mutexes_[i]);
			buckets_[i].erase(to_vertex);

		}
		if (j < buckets_.size() && j >= 0)
		{
			std::lock_guard<std::mutex> lock(bucket_mutexes_[j]);
			buckets_[j].insert(to_vertex);
		}

		/*relax_bucket_mutex_.unlock();*/

		dist_[to_vertex] = tentative_dist;
		pred_[to_vertex] = from_vertex;
	}
}



void DeltaStepParallel::print_light_and_heavy_edges()
{
	std::cout << "Light edges: " << std::endl;
	for (int i = 0; i < graph_.getGraphNbVertices(); i++)
	{
		std::cout << "Vertex " << i << ": ";
		for (const int light_edge : light_edges_[i])
		{
			std::cout << light_edge << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Heavy edges: " << std::endl;
	for (int i = 0; i < graph_.getGraphNbVertices(); i++)
	{
		std::cout << "Vertex " << i << ": ";
		for (const int heavy_edge : heavy_edges_[i])
		{
			std::cout << heavy_edge << " ";
		}
		std::cout << std::endl;
	}
}

void DeltaStepParallel::print_all_buckets() const
{
	for (size_t bucket_id = 0; bucket_id < buckets_.size(); bucket_id++)
	{
		this->print_bucket(bucket_id);
	}
}

void DeltaStepParallel::print_bucket(const size_t bucket_id) const
{
	std::cout << "Bucket [" << bucket_id << "], size " << buckets_[bucket_id].size() << ": ";
	for (const int bucket_item : buckets_[bucket_id])
	{
		std::cout << bucket_item << " ";
	}
	std::cout << std::endl;
}

void find_bucket


void DeltaStepParallel::resolve_requests(const std::vector<Edge>* requests, const size_t begin, const size_t end)
{
	
	for (size_t i = begin; i < end; i++)
	{
		this->relax((*requests)[i]);
	}
}



void DeltaStepParallel::solve()
{

	bucket_mutexes_ = std::vector<std::mutex>(buckets_.size());
	while (bucket_counter_ < buckets_.size())
	{
		std::vector<std::vector<Edge>> light_requests = std::vector<std::vector<Edge>>(graph_.getGraphNbVertices(), std::vector<Edge>());
		std::vector<std::vector<Edge>> heavy_requests = std::vector<std::vector<Edge>>(graph_.getGraphNbVertices(), std::vector<Edge>());
		int nb_light_requests = 0;
		int nb_heavy_requests = 0;
		std::set<int> current_bucket = buckets_[bucket_counter_];
		while (!current_bucket.empty())
		{
			// Make sure that the number of threads is not greater than the number of vertices in the current bucket
			// to ensure that each thread will have at least one vertex to process
			// 
			find_bucket_requests_seq(current_bucket, &light_requests, &heavy_requests);
			nb_light_requests += light_requests.size();
			nb_heavy_requests += heavy_requests.size();
			// Resolve the light requests in parallel
			if(nb_light_requests > 0)
			{
				for (int i = 0; i < graph_.getGraphNbVertices(); i++)
				{
					if (!light_requests[i].empty())
					{
						size_t req_threads;
						if (thread_number_ > light_requests[i].size())
						{
							req_threads = light_requests[i].size();
						}
						else
						{
							req_threads = thread_number_;
						}

						std::vector <std::thread> resolve_light_request_workers(req_threads - 1);
						const size_t light_request_chunk_size = light_requests[i].size() / req_threads;

						for (size_t j = 0; j < req_threads - 1; j++)
						{
							resolve_light_request_workers[j] = std::thread(
								&DeltaStepParallel::resolve_requests,
								this,
								&light_requests[i],
								j * light_request_chunk_size,
								(j + 1) * light_request_chunk_size
							);
						}

						resolve_requests(&light_requests[i], (req_threads - 1) * light_request_chunk_size, light_requests[i].size());

						for (auto& worker : resolve_light_request_workers)
						{
							worker.join();
						}
						light_requests[i].clear();
					}
					
				}
				nb_light_requests = 0;
			}
			
			current_bucket = buckets_[bucket_counter_];
		}
		// Resolve the heavy requests in parallel
		if (nb_heavy_requests > 0)
		{
			for (int i = 0; i < graph_.getGraphNbVertices(); i++)
			{
				if (!heavy_requests[i].empty())
				{
					size_t req_threads;
					if (thread_number_ > heavy_requests[i].size())
					{
						req_threads = heavy_requests[i].size();
					}
					else
					{
						req_threads = thread_number_;
					}

					std::vector <std::thread> resolve_heavy_request_workers(req_threads - 1);
					const size_t heavy_request_chunk_size = heavy_requests[i].size() / req_threads;

					for (size_t j = 0; j < req_threads - 1; j++)
					{
						resolve_heavy_request_workers[j] = std::thread(
							&DeltaStepParallel::resolve_requests,
							this,
							&heavy_requests[i],
							j * heavy_request_chunk_size,
							(j + 1) * heavy_request_chunk_size
						);
					}

					resolve_requests(&heavy_requests[i], (req_threads - 1) * heavy_request_chunk_size, heavy_requests[i].size());

					for (auto& worker : resolve_heavy_request_workers)
					{
						worker.join();
					}
					heavy_requests[i].clear();
				}
			}
			nb_heavy_requests = 0;

		}
		bucket_counter_++;
		while (bucket_counter_ < buckets_.size() && buckets_[bucket_counter_].empty() == true) {
			bucket_counter_++;
		}
	}
}

void DeltaStepParallel::print_solution()
{
	std::cout << "Solution: " << std::endl;
	for (int i = 0; i < graph_.getGraphNbVertices(); i++)
	{
		std::cout << "Vertex " << i << ": " << dist_[i] << std::endl;
	}
}




