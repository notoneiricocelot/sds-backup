#pragma once
#include "Common.h"

namespace SDS
{
	class Specialization
	{
	public:
		Specialization() = default;

		Specialization(std::string i, std::string n, std::string d, SDSFocus f, SDSAttribute a1, SDSAttribute a2)
		{
			_ID = i;
			Name = n;
			Description = d;
			focus = f;
			firstAttribute = a1;
			secondAttribute = a2;
		}

		bool operator==(const Specialization& other) const
		{
			return _ID == other._ID;
		}

		static inline std::vector<std::unique_ptr<Specialization>> Specializations;

		std::string_view GetID() const { return _ID; };
		std::string_view GetName() const { return Name; };
		std::string_view GetDescription() const { return Description; };

		SDSFocus GetFocus() const { return focus; }

		SDSAttribute GetFirstAttribute() const { return firstAttribute; }

		SDSAttribute GetSecondAttribute() const { return secondAttribute; }

		int* GetSkills() { return skills; }

		void SetSkillFocus(int avID, int pos)
		{
			skills[pos] = avID;
		}

		void ToGfxValue(RE::GFxValue* val, RE::GPtr<RE::GFxMovieView> uiMovie);

	protected:
		std::string _ID;

	private:
		std::string Name;
		std::string Description;
		SDSFocus focus;
		SDSAttribute firstAttribute;
		SDSAttribute secondAttribute;
		int skills[6];
	};

}
