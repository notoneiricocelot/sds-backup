#include "Specialization.h"

namespace SDS
{
	void Specialization::ToGfxValue(RE::GFxValue* val, RE::GPtr<RE::GFxMovieView> uiMovie)
	{
		RE::GFxValue s;
		uiMovie->CreateArray(&s);

		val->SetMember("id", RE::GFxValue(_ID));
		val->SetMember("name", RE::GFxValue(Name));
		val->SetMember("description", RE::GFxValue(Description));
		val->SetMember("focus", RE::GFxValue(static_cast<int>(focus)));
		val->SetMember("attr1", RE::GFxValue(static_cast<int>(firstAttribute)));
		val->SetMember("attr2", RE::GFxValue(static_cast<int>(secondAttribute)));

		for (int i = 0; i < 6; i++)
		{
			s.PushBack(skills[i]);
		}
		val->SetMember("skills", s);
	}

	int Specialization::Find(std::string id)
	{
		int i = 0;
		for (auto const& it : Specializations)
		{
			if (it->GetID().compare(id) == 0)
				return i;
			i += 1;
		}
		return -1;
	}
}
