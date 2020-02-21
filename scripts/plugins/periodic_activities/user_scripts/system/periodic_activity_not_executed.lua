--
-- (C) 2019-20 - ntop.org
--

local alert_consts = require("alert_consts")
local alerts_api = require("alerts_api")
local user_scripts = require("user_scripts")

local script

-- ##############################################

local function alert_info(ps_name, last_queued_time)
  return({
    alert_type = alert_consts.alert_types.alert_periodic_activity_not_executed,
    alert_severity = alert_consts.alert_severities.warning,
    alert_granularity = alert_consts.alerts_granularities.min,
    alert_subtype = ps_name,
    alert_type_params = {
       ps_name = ps_name,
       last_queued_time = last_queued_time
    },
  })
end

-- #################################################################

local function check_periodic_activity_not_executed(params)
   local scripts_stats = interface.getPeriodicActivitiesStats()

   for ps_name, ps_stats in pairs(scripts_stats) do
      local delta = alerts_api.interface_delta_val(script.key..ps_name --[[ metric name --]], params.granularity, ps_stats["num_not_executed"] or 0)
      local info = alert_info(ps_name, ps_stats["last_queued_time"] or 0)

      if delta > 0 then
	 -- tprint({ps_name = ps_name, s = ">>>>>>>>>>>>>>>>>>>>>> TRIGGER"})
      	 alerts_api.trigger(params.alert_entity, info, nil, params.cur_alerts)
      else
	 -- tprint({ps_name = ps_name, s = "---------------------- RELEASE"})
      	 alerts_api.release(params.alert_entity, info, nil, params.cur_alerts)
      end
   end
end

-- #################################################################

script = {
  -- Script category
  category = user_scripts.script_categories.internals,

  -- This script is only for alerts generation
  is_alert = true,

  hooks = {
    min = check_periodic_activity_not_executed,
  },

  gui = {
    i18n_title = "alerts_dashboard.periodic_activity_not_executed",
    i18n_description = "alerts_dashboard.periodic_activity_not_executed_descr",
  }
}

-- #################################################################

return script
