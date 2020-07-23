--
-- (C) 2014-20 - ntop.org
--

local lua_path_utils = {}

-- ########################################################

function lua_path_utils.package_path_preprend(path)
   local include_path = path.."/?.lua;"

   -- If the path is already inside package.path, we remove it, before prepending it
   if not package.path:starts(include_path) and package.path:gmatch(include_path) then
      package.path = package.path:gsub(include_path, '')
   end

   package.path = include_path..package.path
end

-- ########################################################

return lua_path_utils

