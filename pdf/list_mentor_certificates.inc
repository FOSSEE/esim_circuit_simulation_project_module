<?php
function _list_circuit_simulation_custom_certificates()
{
	global $user;
	$query_id = db_query("SELECT id FROM esim_circuit_simulation_proposal WHERE approval_status=3");
	$exist_id = $query_id->fetchObject();
	if ($exist_id){
	if ($exist_id->id) {
		if ($exist_id->id < 3) {
			drupal_set_message('<strong>There are no proposals with mentors</strong>', 'status');
			return '';
		} //$exist_id->id < 3
		else {
			$search_rows = array();
			global $output;
			$output = '';
			$query3 = db_query("SELECT id,project_guide_name,project_title FROM esim_circuit_simulation_proposal WHERE project_guide_name != '' AND approval_status=3");
			$i = 1;
			while ($search_data3 = $query3->fetchObject()) {
					$search_rows[] = array(
						$i,
						$search_data3->project_title,
						$search_data3->project_guide_name,
						l('Download Certificate', 'circuit-simulation-project/certificates-custom/pdf/' . $search_data3->id)
					);
					$i++;
				 //$search_data3->id
			} //$search_data3 = $query3->fetchObject()
			if ($search_rows) {
				$search_header = array(
					'No',
					'Project Title',
					'Project Guide Name',
					'Download Certificates'
				);
				$output        = theme('table', array(
					'header' => $search_header,
					'rows' => $search_rows
				));
				return $output;
			} //$search_rows
			else {
				echo ("Error");
				return '';
			}
		}
		}
	} //$exist_id->id
	else {
		drupal_set_message('<strong>There are no proposals with mentors</strong>', 'status');
		$page_content = "<span style='color:red;'> No certificate available </span>";
		return $page_content;
	}
}
