#ifndef SE7_NETWORKING
#define SE7_NETWORKING

#include <map>
#include <string>
#include <string_view>
#include <cstdint>
#include <initializer_list>
#include <utility>
#include <vector>
#include <concepts>
#include <stdexcept>

namespace SE7 {
	class map_insertion_error : public std::runtime_error {
	public:
		map_insertion_error() : std::runtime_error("map insertion error") {}
	};

	namespace networking {
		using html_node_id = std::uintmax_t;
		using html_attribute_id = std::uintmax_t;

		constexpr html_node_id html_id = 0U;
		constexpr html_node_id head_id = 1U;
		constexpr html_node_id body_id = 2U;
		constexpr html_node_id title_id = 3U;
		constexpr html_node_id script_id = 4U;
		// TODO: Continue...

		constexpr html_attribute_id id_id = 0U;
		constexpr html_attribute_id class_id = 1U;

		class html_node;

		class html_attribute {
		private:
			const html_attribute_id attribute_id;
			std::string name;
			std::string data;

			friend html_node;
		public:
			html_attribute(
				html_attribute_id attr_id, 
				std::string_view nm,
				std::string_view d = ""
			) : attribute_id(attr_id), name(nm), data(d) {}
			
			std::string_view get_attribute_name() const {
				return this->name;
			}
			
			std::string_view get_data() const {
				return this->data;
			}
		};

		template<class T>
		concept html_attribute_compatible = std::same_as<T, html_attribute>;

		html_attribute make_html_attribute_id_attribute(std::string_view data) {
			return html_attribute(id_id, "id", data);
		}

		html_attribute make_html_attribute_class_attribute(std::string_view data) {
			return html_attribute(class_id, "class", data);
		}

		class html_node {
		private:
			const html_node_id node_id;
			const std::string name;
			std::map<html_attribute_id, html_attribute> attributes;
			std::map<html_node_id, html_node> children;

			friend html_attribute;

			std::string to_string_all_including_sub_tags_impl(
				bool include_indentation_and_newlines = false,
				bool quote_attributes = true,
				std::string indentation = ""
			) {
				std::string data = (
					(include_indentation_and_newlines ? "\n" : "") +
					(include_indentation_and_newlines ? indentation : "") +
					this->get_open_tag_string(quote_attributes)
				);

				for (std::pair<const html_node_id, html_node> &p : this->children) {
					data += p.second.to_string_all_including_sub_tags_impl(
						include_indentation_and_newlines,
						quote_attributes,
						indentation + "\t"
					);
				}

				data += (
					(include_indentation_and_newlines ? "\n" : "") +
					(include_indentation_and_newlines ? indentation : "") +
					this->get_close_tag_string()
				);

				return data;
			}
		public:
			html_node(
				html_node_id nid,
				std::string_view nm,
				std::initializer_list<std::pair<const html_attribute_id, html_attribute>> attr = {},
				std::initializer_list<std::pair<const html_node_id, html_node>> ch = {}
			) : node_id(nid), name(nm), attributes(attr), children(ch) {}

			html_node &add_child(html_node &&hn) {
				if (auto [it, success] = this->children.insert({ hn.node_id, hn }); !success) {
					throw map_insertion_error();
				}

				return *this;
			}

			html_node &add_attribute(html_attribute &&ha) {
				if (auto [it, success] = this->attributes.insert({ ha.attribute_id, ha }); !success) {
					throw map_insertion_error();
				}

				return *this;
			}

			html_node get_child(html_node_id node_id) const {
				return this->children.at(node_id);
			}

			std::string_view get_name() const {
				return this->name;
			}

			std::string get_open_tag_string(bool quotes = true) const {
				std::string open_tag = "<" + this->name + " ";

				for (const std::pair<const html_attribute_id, html_attribute> &p : this->attributes) {
					open_tag += (
						std::string(p.second.get_attribute_name()) + 
						"=" +
						(quotes ? "\"" : "") +
						std::string(p.second.get_data()) +
						(quotes ? "\"" : "") +
						" "
					);
				}

				open_tag = open_tag.erase(open_tag.size() - 1) + ">";

				return open_tag;
			}

			std::string get_close_tag_string() const {
				return "</" + this->name + ">";
			}

			std::string to_string_all_including_sub_tags(
				bool include_indentation_and_newlines = false,
				bool quote_attributes = true
			) {
				if (this->children.empty()) {
					if (include_indentation_and_newlines) {
						return this->get_open_tag_string(quote_attributes) +
							"\n" + 
							this->get_close_tag_string();
					}

					return this->get_open_tag_string(quote_attributes) + 
						this->get_close_tag_string();
				}

				std::string data = this->to_string_all_including_sub_tags_impl(
					include_indentation_and_newlines,
					quote_attributes
				);

				if (data[0] == '\n') {
					data.erase(0, 1);
				}

				return data;
			}

			std::vector<html_node> get_children() {
				std::vector<html_node> v_h_n;

				for (std::pair<const html_node_id, html_node> &p : this->children) {
					v_h_n.push_back(p.second);
				}

				return v_h_n;
			}

			html_attribute get_attribute(html_attribute_id attr_id) const {
				return this->attributes.at(attr_id);
			}

			std::vector<html_attribute> get_all_attributes() {
				std::vector<html_attribute> v_h_a;

				for (std::pair<const html_node_id, html_attribute> &p : this->attributes) {
					v_h_a.push_back(p.second);
				}

				return v_h_a;
			}
		};

		html_node make_html_node_html() {
			return html_node(html_id, "html");
		}

		html_node make_html_node_head() {
			return html_node(head_id, "head");
		}

		html_node make_html_node_body() {
			return html_node(body_id, "body");
		}

		html_node make_html_node_title() {
			return html_node(title_id, "title");
		}

		html_node make_html_node_script() {
			return html_node(script_id, "script");
		}

		class html_doc {
		private:
			html_node root_node;
		public:
			html_doc() : root_node(make_html_node_html()) {}

			html_doc &add_child(html_node &&hn) {
				this->root_node.add_child(std::move(hn));

				return *this;
			}
		};
	}
}

#endif
