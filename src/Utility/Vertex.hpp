/*
 * POOSV EPFL 2022
 * Marco Antognini
 */

#ifndef INFOSV_VERTEX_HPP
#define INFOSV_VERTEX_HPP

#include <JSON/JSON.hpp>

#include <SFML/Graphics/Vertex.hpp>

#include <vector>

/*!
 * @brief Generate vertexes for world rendering
 *
 * @param textures JSON object for the grass, rock and water textures' filename
 * @param nbCell the size of the world side in cells
 * @param cellSize the size in pixel on the side of a cell
 *
 * @return a vector of vertexes for rendering
 */
std::vector<sf::Vertex> generateVertexes(j::Value const& textures, int nbCell, float cellSize);

/*!
 * @brief Get the first index of the four contiguous indexes for the given cell
 *
 * @note This assume the vertexes were created with generateVetexes()
 */
std::size_t startIndexForCellVertexes(int x, int y, int nbCell);

/*!
 * @brief Range of indexes for the given cell position
 *
 * @note This assume the vertexes were created with generateVetexes()
 */
std::vector<std::size_t> indexesForCellVertexes(int x, int y, int nbCell);

#endif // INFOSV_VERTEX_HPP
