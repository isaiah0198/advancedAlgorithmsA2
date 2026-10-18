# Empirical Comparison of Karger’s Randomised Contraction Algorithm and the Stoer–Wagner Algorithm for Global Minimum Cut

## 1. Introduction

The global minimum cut problem asks for the minimum total edge weight that must be removed from an undirected graph to divide the graph into two non-empty disconnected components. The problem provides a useful setting for comparing deterministic and randomized algorithms because the same target result can be approached using substantially different strategies.

This project compares two global minimum cut algorithms: **Stoer–Wagner**, a deterministic contraction-based algorithm, and **Karger’s randomised contraction algorithm**. Stoer–Wagner repeatedly performs maximum-adjacency search phases and contracts vertices while tracking candidate cuts. Karger instead repeatedly contracts randomly selected edges until only two supervertices remain. Because Karger may contract an edge belonging to a minimum cut, a single run is not guaranteed to find the optimal answer.

The purpose of this project was not only to implement both algorithms, but to investigate how their practical behaviour changes under different graph conditions. The central research question was:

**How do graph size, graph density and repetition count affect the runtime and reliability of Karger’s randomised contraction algorithm compared with deterministic Stoer–Wagner for global minimum cut?**

The investigation focused on three areas:

- runtime as the number of vertices increased;
- Karger reliability as graph density changed;
- the relationship between Karger repetition count, runtime and probability of finding the true minimum cut.

## 2. Algorithm 1: Stoer–Wagner

**Primary author: Isaiah Lane**

### 2.1 Algorithm Overview

Stoer–Wagner is a deterministic algorithm for finding the global minimum cut of an undirected weighted graph. The original algorithm consists of approximately $|V|$ maximum-adjacency search phases.

During each phase, the algorithm begins with an arbitrary active vertex and repeatedly adds the vertex that has the greatest total connection weight to the vertices already selected.

If $A$ is the set of selected vertices, each unselected vertex is assigned a connection weight equal to the sum of edge weights linking it to vertices in $A$. The vertex with the maximum connection weight is selected next.

The final two vertices added during the phase can be described as $s$ and $t$. When $t$ is added last, its accumulated connection weight is the weight of a valid cut separating $t$ from the vertices selected before it. This produces a candidate cut for the global minimum.

The vertices $s$ and $t$ are then contracted. Their incident edge weights are combined, and the algorithm repeats on the smaller graph.

The smallest candidate cut encountered over all phases is returned as the global minimum cut.

### 2.2 Why a Phase Produces a Candidate Cut

At the end of a maximum-adjacency phase, the final selected vertex $t$ is separated from all vertices selected before it.

Its accumulated connection weight therefore represents the total edge weight crossing the partition:

$$
\{t\}, \quad V \setminus \{t\}
$$

within the current contracted graph.

This is a valid cut. Stoer–Wagner’s correctness argument shows that the minimum $s$-$t$ cut for the final two vertices of a phase is represented by this phase cut, and contracting $s$ and $t$ allows the remaining global minimum-cut possibilities to be preserved for later phases.

This process is repeated until the graph has been reduced to one active supervertex.

### 2.3 Complexity of the Implementation

The implementation in this project uses an adjacency matrix and simple linear scans.

Each phase selects up to $O(n)$ vertices. Selecting the next vertex requires scanning up to $O(n)$ candidates, and there are $O(n)$ phases.

Therefore, the implementation is approximately:

$$
\Theta(n^3)
$$

in time.

The adjacency matrix requires:

$$
\Theta(n^2)
$$

memory.

This complexity describes the implementation developed for this project rather than every possible implementation of Stoer–Wagner.

## 3. Algorithm 2: Karger’s Randomised Contraction

**Primary author: Isaiah Lane**

### 3.1 Algorithm Overview

Karger’s algorithm solves the same problem using random edge contraction.

At each step:

1. an edge is selected randomly;
2. the endpoints are merged into one supervertex;
3. self-loops are ignored;
4. parallel edges are preserved;
5. contraction continues until only two active supervertices remain.

The total weight of the edges between the final two supervertices is returned as the cut for that run.

The key risk is that a true minimum-cut edge may be contracted before the algorithm finishes. Once this happens, the corresponding minimum cut cannot be recovered during that run.

This makes Karger probabilistic rather than deterministic.

### 3.2 Single-Run Success Bound

For a graph with $n$ vertices, the classical worst-case lower bound on the probability that basic random contraction preserves a particular minimum cut is:

$$
p \geq \frac{2}{n(n-1)}
$$

For $n=50$:

$$
p \geq \frac{2}{50 \times 49}
$$

$$
p \geq 0.000816
$$

or approximately:

$$
0.0816\%
$$

This is a worst-case guarantee rather than a prediction that all 50-vertex graphs will have such a low success probability.

The experimental results in this project were substantially better than this lower bound.

### 3.3 Repeated Karger

If the probability of a single Karger run finding the minimum cut is $p$, then the probability that all $r$ independent runs fail is:

$$
(1-p)^r
$$

Therefore the probability that at least one run succeeds is:

$$
P_r = 1-(1-p)^r
$$

This provides a direct theoretical prediction for the repetition experiments.

For example, with a single-run success probability of:

$$
p=0.144
$$

and ten repetitions:

$$
P_{10}=1-(1-0.144)^{10}
$$

which gives approximately:

$$
78.9\%
$$

The observed result in the experiment was **78.8%**.

### 3.4 Complexity of the Project Implementation

A key limitation of this project is that the Karger implementation does not use the most efficient possible edge-sampling representation.

To select a random edge, the program:

1. scans active vertex pairs to calculate the total edge weight;
2. generates a random target;
3. scans the matrix again to identify the corresponding edge.

This costs approximately:

$$
O(n^2)
$$

per contraction.

Approximately $n-2$ contractions are required per run, giving:

$$
\Theta(n^3)
$$

for one Karger execution in this implementation.

With $r$ repetitions:

$$
\Theta(rn^3)
$$

This distinction is important because a more efficient Karger implementation can avoid repeatedly rescanning the full adjacency matrix. The runtime comparison in this project should therefore be interpreted as a comparison of the **submitted implementations**, rather than a claim that basic randomised contraction must always be slower than Stoer–Wagner.

## 4. Theoretical Comparison and Predictions

This section was defined before interpreting the experimental results.

Based on the implementations, four predictions were made.

First, both algorithms were expected to show approximately cubic growth with vertex count because both implementations repeatedly scan an $n \times n$ adjacency matrix inside contraction-related loops.

Therefore:

$$
T_{SW}(n) \approx \Theta(n^3)
$$

and for fixed Karger repetition count $r$:

$$
T_K(n) \approx \Theta(n^3)
$$

Second, Karger runtime was expected to increase approximately linearly with repetition count because each repetition performs another full contraction run:

$$
T_K(r) \propto r
$$

Third, Karger reliability was expected to increase according to:

$$
1-(1-p)^r
$$

where $p$ is the single-run success probability.

Fourth, graph density was expected to have a smaller effect on runtime than vertex count. This is because the adjacency-matrix implementations scan candidate vertex pairs whether or not every possible edge actually exists.

No prediction was made that density itself would necessarily decrease Karger reliability. Reliability depends on graph structure and the arrangement and multiplicity of minimum cuts, so the density experiment was exploratory.

## 5. Implementation

Both algorithms were implemented in C++17 using the same `Graph` class.

### 5.1 Graph Representation

The graph is represented using an adjacency matrix.

For vertices $u$ and $v$:

$$
weights[u][v]
$$

stores their edge weight.

Because the graph is undirected:

$$
weights[u][v] = weights[v][u]
$$

The class also stores an `active` flag for each vertex. Contracted vertices are marked inactive rather than physically deleting matrix rows and columns.

This simplified contraction and allowed vertex indices to remain constant throughout execution.

### 5.2 Contraction

When vertex $t$ is contracted into $s$, the implementation updates each active neighbouring vertex $v$ using:

$$
w(s,v) = w(s,v) + w(t,v)
$$

The reverse matrix position is then assigned the same value to preserve symmetry.

Vertex $t$ is marked inactive.

Parallel edges created through contraction are therefore represented by increasing matrix weights.

### 5.3 Random Edge Selection

Karger must select individual edges rather than simply choosing uniformly from non-zero matrix cells.

If a matrix cell has weight four after contraction, it represents four parallel edges and should be four times as likely to be selected as a cell with weight one.

The implementation therefore:

1. totals all active edge weights;
2. selects a random integer within that total;
3. scans cumulative edge weights until the target is reached.

Randomness is produced using `std::mt19937`.

Explicit seeds were used to make experimental runs reproducible.

## 6. Validation and Debugging

Correctness was checked before running the final benchmarks.

The test graphs included:

| Graph | Minimum cut |
|---|---:|
| Weighted triangle | 5 |
| Four-vertex path | 1 |
| Four-vertex cycle | 2 |
| Five-vertex weighted graph | 5 |

Both algorithms recovered these expected values when Karger was repeated sufficiently.

Additional validation compared Stoer–Wagner against brute-force minimum-cut enumeration on 3,000 randomly generated weighted and unweighted graphs containing up to 11 vertices. No mismatches were found. Karger also never returned a cut lower than the exact brute-force minimum in these tests.

This was particularly important because Stoer–Wagner was used as the exact oracle for the later Karger reliability experiments.

Several development bugs were found incrementally.

One early bug caused `addEdge()` to update only one direction of the adjacency matrix, violating the graph’s undirected symmetry.

Another occurred during contraction, when independently adding both symmetric entries caused incorrect edge weights.

The most significant bug occurred in Stoer–Wagner vertex selection. An incorrect boolean condition skipped active vertices instead of inactive vertices and eventually caused invalid selection and a segmentation fault. AddressSanitizer was used to locate the failure, after which temporary diagnostic output identified the incorrect active-state logic.

A later experimental bug was not an algorithm bug at all. The density benchmark was accidentally nested inside the graph-size loop. The program compiled and produced plausible-looking results, but timings were associated with the wrong experimental conditions. The experiment structure was then corrected.

These errors showed why algorithm testing required more than successful compilation.

## 7. Experimental Methodology

### 7.1 Build Configuration

The final benchmarks were compiled using C++17 and optimisation:

```text
-std=c++17 -O2
```

AddressSanitizer was used during debugging but excluded from the final performance measurements because instrumentation adds substantial overhead.

The exact compiler version and hardware specification should be recorded in the final submitted version using the system information from the machine on which the benchmark was run.

No explicit warm-up phase was used. Each runtime condition was instead executed 51 times, and the median was reported. Using a median reduces sensitivity to occasional unusually high or low timings, although it does not completely remove possible cold-start effects.

### 7.2 Runtime Measurement

Runtime was measured using:

```cpp
std::chrono::high_resolution_clock
```

Each timing condition contained 51 measurements.

The median measurement was used as the representative runtime.

### 7.3 Graph Generation

Generated experimental graphs were guaranteed to be connected.

First, a random spanning tree was created by connecting every new vertex to one randomly selected earlier vertex.

Additional undirected edges were then inserted until the target edge count was reached.

Self-loops and duplicate edges were rejected.

All generated benchmark graphs used unit edge weights.

## 8. Experimental Design

### 8.1 Repetition Reliability

Karger was tested on the five-vertex weighted graph using:

$$
1,\ 2,\ 5,\ 10,\ 20,\ 50
$$

repetitions.

Each condition contained 100 experiments.

A trial was successful if Karger returned the same cut as Stoer–Wagner.

### 8.2 Size Scaling

Graphs containing:

$$
10,\ 25,\ 50,\ 100
$$

vertices were generated.

The number of additional edges was set equal to the vertex count to keep this graph family relatively sparse.

Stoer–Wagner was compared with Karger using 20 repetitions.

### 8.3 Density

The number of vertices was fixed at 50.

The tested densities were:

$$
0.10,\ 0.25,\ 0.50,\ 0.75
$$

corresponding to:

| Density | Edges |
|---:|---:|
| 0.10 | 122 |
| 0.25 | 306 |
| 0.50 | 612 |
| 0.75 | 918 |

Stoer–Wagner supplied the exact minimum cut for each graph.

Karger single-run reliability was measured over 100 trials.

### 8.4 Multiple Graph Instances

To reduce dependence on a single generated topology, five graph-generation seeds were used:

$$
42,\ 142,\ 242,\ 342,\ 442
$$

For each density, 100 Karger trials were performed on each graph.

This produced:

$$
500
$$

single-run observations per density.

### 8.5 Repetitions Across Density

Karger was tested with:

$$
1,\ 10,\ 20
$$

repetitions.

Five graph instances were used per density and 50 experiments were run per graph, producing:

$$
250
$$

observations for each density/repetition condition.

## 9. Results

### 9.1 Reliability Versus Repetition Count

| Repetitions | Successes | Success rate |
|---:|---:|---:|
| 1 | 33 / 100 | 33% |
| 2 | 43 / 100 | 43% |
| 5 | 80 / 100 | 80% |
| 10 | 94 / 100 | 94% |
| 20 | 100 / 100 | 100% |
| 50 | 100 / 100 | 100% |

Reliability increased strongly with repetition count.

However, the 100% observed success at 20 repetitions applies only to this five-vertex graph and finite test sample. Later experiments showed that 20 repetitions were not sufficient to achieve comparable reliability on denser 50-vertex graphs.

### 9.2 Runtime Versus Repetition Count

| Algorithm | Median runtime |
|---|---:|
| Stoer–Wagner | 1,292 ns |
| Karger ×1 | 2,750 ns |
| Karger ×2 | 5,417 ns |
| Karger ×5 | 13,542 ns |
| Karger ×10 | 27,042 ns |
| Karger ×20 | 46,917 ns |
| Karger ×50 | 117,292 ns |

Karger runtime rose approximately linearly with repetition count, matching the prediction that each repetition adds another complete contraction execution.

## 10. Graph Size Results

| Vertices | Exact cut | Stoer–Wagner median | Karger ×20 median |
|---:|---:|---:|---:|
| 10 | 2 | 5,459 ns | 89,167 ns |
| 25 | 1 | 90,166 ns | 789,042 ns |
| 50 | 1 | 620,334 ns | 4,585,625 ns |
| 100 | 1 | 5,086,334 ns | 30,232,917 ns |

Both algorithms showed rapid growth as the number of vertices increased.

For Stoer–Wagner, doubling graph size from 50 to 100 vertices increased runtime from approximately 0.62 ms to 5.09 ms, a factor of approximately eight.

Since:

$$
2^3 = 8
$$

this is consistent with the predicted cubic behaviour of the implementation.

Karger ×20 also displayed behaviour broadly consistent with cubic scaling when repetition count was held constant.

The ratio between Karger ×20 and Stoer–Wagner fell from roughly sixteen times at $n=10$ to roughly six times at $n=100$.

This should not be interpreted as evidence that the Karger implementation has better asymptotic complexity. Both implementations are approximately cubic. Fixed per-run overheads such as constructing `mt19937`, copying graph state and repeatedly entering the contraction routine are likely to contribute more heavily to runtime on smaller graphs.

## 11. Density Results

### 11.1 Minimum Cut

| Density | Edges | Minimum cut |
|---:|---:|---:|
| 0.10 | 122 | 1 |
| 0.25 | 306 | 5 |
| 0.50 | 612 | 17 |
| 0.75 | 918 | 28 |

The minimum-cut value increased as density increased for the graph generated using seed 42.

### 11.2 Runtime

| Density | Stoer–Wagner | Karger ×20 |
|---:|---:|---:|
| 0.10 | 648,292 ns | 4,571,041 ns |
| 0.25 | 667,750 ns | 4,855,167 ns |
| 0.50 | 673,000 ns | 4,817,291 ns |
| 0.75 | 674,208 ns | 4,921,625 ns |

Density had relatively little effect on runtime compared with graph size.

This matches the theoretical prediction based on the implementation.

Both algorithms repeatedly scan ranges of possible vertices in the adjacency matrix. Increasing the number of actual edges therefore does not increase work as dramatically as increasing the matrix dimension.

## 12. Karger Reliability and Density

Using one graph generated with seed 42:

| Density | Success |
|---:|---:|
| 0.10 | 60% |
| 0.25 | 13% |
| 0.50 | 6% |
| 0.75 | 6% |

The experiment was repeated across five generated graphs.

| Density | Successes | Success rate |
|---:|---:|---:|
| 0.10 | 221 / 500 | 44.2% |
| 0.25 | 69 / 500 | 13.8% |
| 0.50 | 30 / 500 | 6.0% |
| 0.75 | 34 / 500 | 6.8% |

The decline therefore remained after introducing additional graph instances.

However, density itself should not be interpreted as the direct causal factor.

Karger succeeds when at least one minimum cut survives all contractions. The probability therefore depends on structural properties such as the number of equivalent minimum cuts and how those cuts are distributed throughout the graph.

One possible explanation is that sparse graphs in this generator may contain several low-degree vertices, producing multiple simple minimum cuts. This would provide Karger with more ways to return an optimal result.

Denser graphs may instead contain a more structurally specific minimum cut.

This explanation was not directly tested in the current experiment and therefore remains a hypothesis rather than a demonstrated result.

## 13. Repetition Probability: Theory Versus Experiment

This produced one of the strongest results in the project.

The repeated-run model predicts:

$$
P_r=1-(1-p)^r
$$

At density 0.25, the observed single-run success probability was:

$$
p=0.144
$$

For ten repetitions:

$$
1-(1-0.144)^{10}
\approx 0.789
$$

or:

$$
78.9\%
$$

The experimental result was:

$$
78.8\%
$$

At density 0.75, single-run success was approximately:

$$
p=0.064
$$

Twenty repetitions predict approximately:

$$
73\%
$$

success, while the experiment observed:

$$
72\%
$$

This close agreement shows that repetition behaved approximately as expected from the probabilistic model.

Differences between theoretical predictions calculated from aggregate $p$ and observed results can be explained partly by graph-to-graph variation. Each generated graph may have a different underlying success probability.

## 14. Worst-Case Theory Versus Practice

The more surprising result was how conservative Karger’s classical worst-case guarantee was for these inputs.

For $n=50$:

$$
p \geq \frac{2}{50 \times 49}
$$

which is approximately:

$$
0.0816\%
$$

Yet empirical single-run success ranged from:

$$
6.0\%
$$

to:

$$
44.2\%
$$

across the tested densities.

Therefore, the tested graphs were substantially easier for Karger than the worst-case theoretical guarantee.

Using the worst-case probability, thousands of repetitions would be required to achieve approximately 95% confidence.

Using the observed probabilities, the number required was dramatically lower.

For a target probability $P$, the repetition requirement can be estimated from:

$$
r \geq
\frac{\ln(1-P)}
{\ln(1-p)}
$$

At density 0.50, with:

$$
p=0.06
$$

approximately 49 repetitions are required for around 95% confidence.

This is substantially larger than the 20 repetitions used in the original runtime comparison, but still dramatically lower than would be required under the theoretical worst-case bound.

This demonstrates the difference between a worst-case guarantee and typical performance on a particular graph family.

## 15. Equal-Reliability Runtime Comparison

A fixed Karger repetition count is not always a fair comparison with deterministic Stoer–Wagner.

At density 0.50:

- single-run Karger success was approximately 6%;
- 20 repetitions achieved 76.4%;
- approximately 49 repetitions would be required for about 95% confidence.

Karger ×20 required approximately:

$$
4.82\text{ ms}
$$

at density 0.50.

Assuming approximately linear scaling with repetition count:

$$
4.82
\times
\frac{49}{20}
\approx
11.8\text{ ms}
$$

would be expected for roughly 49 repetitions.

Stoer–Wagner required approximately:

$$
0.67\text{ ms}
$$

and returned an exact deterministic result.

Therefore, for this implementation and graph family, Stoer–Wagner provided a substantially better runtime/reliability trade-off.

This comparison remains implementation-specific because Karger’s matrix-scanning edge selection contributes significantly to its runtime.

## 16. Discussion

The experimental results generally agreed with the theoretical predictions.

Both implementations showed approximately cubic growth with vertex count.

Karger runtime increased roughly linearly with repetition count.

Repeated Karger reliability closely followed:

$$
1-(1-p)^r
$$

The main unexpected finding was the difference between Karger’s pessimistic worst-case theoretical guarantee and its empirical performance.

Although the worst-case single-run probability for $n=50$ is approximately 0.08%, the observed values ranged from 6% to 44%.

This demonstrates why worst-case bounds are useful as guarantees but should not necessarily be interpreted as predictions of typical behaviour.

The density experiments also highlighted the importance of distinguishing graph size from graph structure.

Density barely changed runtime because the adjacency-matrix implementation scans vertex pairs regardless of edge existence.

However, density was strongly associated with Karger reliability in the generated graphs.

This appears more likely to reflect changes in cut structure than density by itself.

Finally, Stoer–Wagner was faster across all tested conditions, but this should not become an overly broad headline.

A major reason is that the Karger implementation performs $O(n^2)$ edge-selection scans during each contraction. A more specialised data structure could produce a much faster implementation.

The correct conclusion is therefore:

> **For the implementations developed in this project, Stoer–Wagner provided better deterministic runtime performance, while Karger demonstrated a clear and measurable trade-off between repetition cost and probabilistic reliability.**

## 17. Limitations

Several limitations affect the interpretation of the results.

First, graph size was tested only up to 100 vertices.

Second, only one graph instance was used for each size-scaling condition. The density reliability experiments were stronger because they included five generated graph instances.

Third, the generated benchmark graphs used unit edge weights. The implementations support weighted graphs and weighted correctness tests were used, but the large random experiments did not explore different weight distributions.

Fourth, many of the sparse size-scaling graphs had trivial global minimum cuts of one. They therefore represent a relatively narrow structural regime.

Fifth, the adjacency matrix strongly influences performance. Sparse graphs receive little runtime benefit because the implementation still scans the full matrix.

Sixth, the Karger implementation is not an optimised textbook implementation. Random-edge sampling requires repeated matrix scans and produces approximately cubic runtime per execution.

Seventh, explicit warm-up runs were not performed.

Eighth, median timings were reported but no interquartile ranges or timing confidence intervals were calculated.

Ninth, deterministic pseudorandom seeds improve reproducibility but do not prove statistical independence between trials.

Finally, only one random graph-generation family was studied. Different graph structures could produce substantially different Karger reliability.

## 18. What We Would Do Next

The next step should not simply be increasing the number of random trials.

A more useful extension would introduce a **planted-cut graph family**.

For example, two internally dense vertex groups could be connected by a controlled number of crossing edges. This would produce a known, non-trivial minimum cut while allowing graph size and internal density to vary independently.

A second extension would test the explanation proposed for the density result.

For each generated graph, properties such as:

- minimum vertex degree;
- number of vertices attaining the minimum degree;
- number of distinct global minimum cuts;

could be recorded.

This would help determine whether Karger performs better on sparse graphs because those graphs contain more equivalent optimal cuts.

A third improvement would replace the current Karger edge-selection method with a representation supporting more efficient random-edge sampling.

This would make the runtime comparison more representative of the underlying randomised algorithm rather than the current adjacency-matrix implementation.

Finally, the benchmarking methodology could be strengthened using:

- explicit warm-up runs;
- multiple generated graphs at every size;
- IQR or confidence intervals;
- larger graph sizes;
- weighted graph families.

## 19. Individual Contributions

This assessment was completed individually.

**Student:** Isaiah Lane

**Stoer–Wagner primary author:** Isaiah Lane

**Karger primary author:** Isaiah Lane

All implementation, debugging, validation, experimental design, benchmarking, analysis and report preparation were completed as part of the individual project.

## 20. AI Use

Generative AI tools were used as a development and learning aid throughout the project.

AI assistance was used to:

- explain Karger and Stoer–Wagner concepts;
- break implementation work into incremental steps;
- discuss likely coding mistakes;
- help interpret compiler and AddressSanitizer output;
- discuss benchmark design;
- suggest reproducible seed strategies;
- structure runtime and reliability experiments;
- assist with CSV output;
- review the report structure and identify analysis gaps.

The implementation was developed and tested incrementally rather than copying a complete generated solution.

Examples of AI-supported debugging included investigation of the incorrect active-vertex condition in Stoer–Wagner, reasoning about adjacency-matrix symmetry during contraction, and identifying an incorrectly nested benchmark loop.

All code was compiled and executed locally.

Algorithm correctness was tested using known-answer graphs and additional brute-force comparisons.

Benchmark values included in the report were generated by the submitted program rather than generated by AI.

AI-generated explanations were reviewed against the implementation, experimental results and original academic sources before inclusion.

The original publications by Karger, Karger and Stein, and Stoer and Wagner were used as the authoritative sources for algorithmic claims rather than AI-generated text.

## 21. Conclusion

This project implemented and empirically compared deterministic Stoer–Wagner and randomised Karger contraction for the global minimum cut problem.

The experiments demonstrated three main findings.

First, runtime scaling was broadly consistent with the implementation-level theoretical prediction that both algorithms behave approximately cubically with vertex count.

Second, Karger reliability increased predictably with repetition count. The observed results closely matched the probabilistic relationship:

$$
1-(1-p)^r
$$

Third, Karger performed substantially better in practice than its classical worst-case success guarantee suggested. For 50-vertex graphs, the worst-case bound is approximately 0.08%, whereas empirical success ranged between 6% and 44% depending on graph density and structure.

The results also showed that graph density had relatively little runtime effect in the adjacency-matrix implementation but was strongly associated with Karger reliability.

Stoer–Wagner was faster than repeated Karger in every tested runtime condition and returned exact results deterministically.

However, this outcome is partly explained by implementation choices. The Karger implementation rescans the adjacency matrix during every contraction, resulting in approximately cubic time per run.

The project therefore does not establish that Stoer–Wagner is universally faster than Karger. Instead, it demonstrates how algorithm theory, data structures, randomisation and repetition interact in a practical implementation.

The most important conclusion is that Karger’s simplicity comes with a measurable probabilistic cost. Repetition can compensate for this, and the observed gains closely follow probability theory, but the number of repetitions required depends strongly on the input graph.

## References

Karger, D. R. (1993). *Global min-cuts in RNC, and other ramifications of a simple min-cut algorithm*. Proceedings of the Fourth Annual ACM-SIAM Symposium on Discrete Algorithms.

Karger, D. R., & Stein, C. (1996). *A new approach to the minimum cut problem*. Journal of the ACM, 43(4), 601–640. https://doi.org/10.1145/234533.234534

Stoer, M., & Wagner, F. (1997). *A simple min-cut algorithm*. Journal of the ACM, 44(4), 585–591. https://doi.org/10.1145/263867.263872
