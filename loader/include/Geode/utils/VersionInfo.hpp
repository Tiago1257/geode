#pragma once

#include "../DefaultInclude.hpp"
#include <string_view>
#include "../external/json/json.hpp"
#include <tuple>

namespace geode {
    enum class VersionCompare {
        LessEq,
        Exact,
        MoreEq,
    };

    /**
     * A version label, like v1.0.0-alpha or v2.3.4-prerelease. Purely semantic, 
     * and not used in comparisons; so for example v1.0.0-alpha == v1.0.0.
     */
    enum class VersionTag {
        Alpha,
        Beta,
        Prerelease,
    };
    GEODE_DLL std::optional<VersionTag> versionTagFromString(std::string const& str);
    GEODE_DLL std::string versionTagToSuffixString(VersionTag tag);
    GEODE_DLL std::string versionTagToString(VersionTag tag);

    /**
     * Class representing version information. Not strictly semver, notably in 
     * regard to identifiers; identifiers are restricted to a few common ones, 
     * and are purely semantic, i.e. not used in comparisons. See VersionTag 
     * for details
     * @class VersionInfo
     */
    class GEODE_DLL VersionInfo final {
    protected:
        size_t m_major = 1;
        size_t m_minor = 0;
        size_t m_patch = 0;
        std::optional<VersionTag> m_tag;

    public:
        constexpr VersionInfo() = default;
        constexpr VersionInfo(size_t major, size_t minor, size_t patch) {
            m_major = major;
            m_minor = minor;
            m_patch = patch;
        }
        constexpr VersionInfo(
            size_t major, size_t minor, size_t patch,
            std::optional<VersionTag> tag
        ) {
            m_major = major;
            m_minor = minor;
            m_patch = patch;
            m_tag = tag;
        }
        VersionInfo(std::string const& versionString);
        static bool validate(std::string const& string);

        constexpr size_t getMajor() const {
            return m_major;
        }

        constexpr size_t getMinor() const {
            return m_minor;
        }

        constexpr size_t getPatch() const {
            return m_patch;
        }

        constexpr std::optional<VersionTag> getTag() const {
            return m_tag;
        }

        // Apple clang does not support operator<=>! Yippee!

        constexpr bool operator==(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch) ==
                std::tie(other.m_major, other.m_minor, other.m_patch);
        }
        constexpr bool operator<(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch) <
                std::tie(other.m_major, other.m_minor, other.m_patch);
        }
        constexpr bool operator<=(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch) <=
                std::tie(other.m_major, other.m_minor, other.m_patch);
        }
        constexpr bool operator>(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch) >
                std::tie(other.m_major, other.m_minor, other.m_patch);
        }
        constexpr bool operator>=(VersionInfo const& other) const {
            return std::tie(m_major, m_minor, m_patch) >=
                std::tie(other.m_major, other.m_minor, other.m_patch);
        }

        std::string toString(bool includeTag = true) const;
    };
    void GEODE_DLL to_json(nlohmann::json& json, VersionInfo const& info);
    void GEODE_DLL from_json(nlohmann::json const& json, VersionInfo& info);
    GEODE_DLL std::ostream& operator<<(std::ostream& stream, VersionInfo const& version);

    class GEODE_DLL ComparableVersionInfo final {
    protected:
        VersionInfo m_version;
        VersionCompare m_compare = VersionCompare::Exact;
    
    public:
        constexpr ComparableVersionInfo() = default;
        constexpr ComparableVersionInfo(
            VersionInfo const& version,
            VersionCompare const& compare
        ) : m_version(version), m_compare(compare) {}
        ComparableVersionInfo(std::string const& versionString);
        static bool validate(std::string const& string);

        constexpr bool compare(VersionInfo const& version) const {
            switch (m_compare) {
                case VersionCompare::Exact:    return m_version == version; break;
                case VersionCompare::LessEq:   return m_version <= version; break;
                case VersionCompare::MoreEq:   return m_version >= version; break;
            }
            return false;
        }

        std::string toString() const;
    };
    void GEODE_DLL to_json(nlohmann::json& json, ComparableVersionInfo const& info);
    void GEODE_DLL from_json(nlohmann::json const& json, ComparableVersionInfo& info);
    GEODE_DLL std::ostream& operator<<(std::ostream& stream, ComparableVersionInfo const& version);
}
