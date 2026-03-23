#include "html_decode.h"

// TODO: узнать про статик

static const std::pair<std::string, char> entities[] = {
	{ "&quot;", '\"' },
	{ "&apos;", '\'' },
	{ "&amp;", '&' },
	{ "&lt;", '<' },
	{ "&gt;", '>' }
};

static constexpr char kSeparatorChar = '&';

size_t FindEntityAtHtml(std::string const& text, const size_t pos, std::string& result)
{
	for (const auto& [entity, replacement] : entities)
	{
		if (pos + entity.length() <= text.length() && text.compare(pos, entity.length(), entity) == 0)
		{
			result += replacement;
			return entity.length();
		}
	}
	result += text[pos];
	return 1;
}

std::string HtmlDecode(std::string const& html)
{
	std::string result;
	result.reserve(html.size());

	for (size_t i = 0; i < html.size();)
	{
		if (html[i] == kSeparatorChar)
		{
			i += FindEntityAtHtml(html, i, result);
		}
		else
		{
			result += html[i];
			++i;
		}
	}
	return result;
}
