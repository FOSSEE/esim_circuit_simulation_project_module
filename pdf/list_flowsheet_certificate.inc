<?php
function _list_flowsheet_certificates()
{
	global $user;
	$query_id = db_query("SELECT id FROM dwsim_flowsheet_proposal WHERE approval_status=3 AND uid= :uid", array(
		':uid' => $user->uid
	));
	$exist_id = $query_id->fetchObject();
	if ($exist_id){
	if ($exist_id->id) {
		if ($exist_id->id < 3) {
			drupal_set_message('<strong>You need to propose a flowsheet <a href="http://dwsim.fossee.in/flowsheeting-project/proposal">Flowsheet Proposal</a></strong> or if you have already proposed then your flowsheet is under reviewing process', 'status');
			return '';
		} //$exist_id->id < 3
		else {
			$search_rows = array();
			global $output;
			$output = '';
			$query3 = db_query("SELECT id,project_title,contributor_name FROM dwsim_flowsheet_proposal WHERE approval_status=3 AND uid= :uid", array(
				':uid' => $user->uid
			));
			while ($search_data3 = $query3->fetchObject()) {
				if ($search_data3->id) {
					$search_rows[] = array(
						$search_data3->project_title,
						$search_data3->contributor_name,
						l('Download Certificate', 'flowsheeting-project/certificates/generate-pdf/' . $search_data3->id)
					);
				} //$search_data3->id
			} //$search_data3 = $query3->fetchObject()
			if ($search_rows) {
				$search_header = array(
					'Project Title',
					'Contributor Name',
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
		drupal_set_message('<strong>You need to propose a flowsheet <a href="http://dwsim.fossee.in/flowsheeting-project/proposal">Flowsheet Proposal</a></strong> or if you have already proposed then your flowsheet is under reviewing process', 'status');
		$page_content = "<span style='color:red;'> No certificate available </span>";
		return $page_content;
	}
}
