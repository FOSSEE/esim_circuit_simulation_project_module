<?php
// $Id$
function circuit_simulation_download_full_project()
{
	global $user;
	$id = arg(3);
	$root_path = circuit_simulation_path();
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $id);
	$circuit_simulation_q = $query->execute();
	$circuit_simulation_data = $circuit_simulation_q->fetchObject();
	$CIRCUITSIMULATION_PATH = $circuit_simulation_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $id);
	$circuit_simulation_udc_q = $query->execute();
	while ($circuit_simulation_udc_row = $circuit_simulation_udc_q->fetchObject()) {
		if ($circuit_simulation_udc_row->samplefilepath != 'NULL') {
			$REFERENCE_PATH = 'Reference_file/';
			$str = substr($circuit_simulation_udc_row->samplefilepath,strrpos($circuit_simulation_udc_row->samplefilepath, '/'));
		$resource_file =ltrim($str, '/');
			$zip->addFile($root_path . $CIRCUITSIMULATION_PATH . $resource_file , $REFERENCE_PATH . str_replace(' ', '_', basename($resource_file)));
		} //$CIRCUITSIMULATION_udc_row->user_defined_compound_filepath || $CIRCUITSIMULATION_udc_row->user_defined_compound_filepath != 'NULL'
	} //$CIRCUITSIMULATION_udc_row = $CIRCUITSIMULATION_udc_q->fetchObject()
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $id);
	//$circuit_simulation_f_q = $query->execute();
	/*while ($circuit_simulation_data) {
		$str = substr($circuit_simulation_data->samplefilepath,strrpos($circuit_simulation_data->samplefilepath, '/'));
		$resource_file =ltrim($str, '/');
		$zip->addFile($root_path . $CIRCUITSIMULATION_PATH . $resource_file);
		//var_dump($root_path . $CIRCUITSIMULATION_PATH . $resource_file);die;
	} //$CIRCUITSIMULATION_f_row = $CIRCUITSIMULATION_f_q->fetchObject()
	*/$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are no circuit simulation project in this proposal to download", 'error');
		drupal_goto('circuit-simulation-project/full-download/project');
	}
}
function circuit_simulation_download_completed_project()
{
		global $user;
	$id = arg(3);
	$root_path = circuit_simulation_path();
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $id);
	$circuit_simulation_q = $query->execute();
	$circuit_simulation_data = $circuit_simulation_q->fetchObject();
	$CIRCUITSIMULATION_PATH = $circuit_simulation_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $id);
	$circuit_simulation_udc_q = $query->execute();
	while ($circuit_simulation_udc_row = $circuit_simulation_udc_q->fetchObject()) {
		if ($circuit_simulation_udc_row->samplefilepath != 'NULL') {
			$REFERENCE_PATH = 'Reference_file/';
			$str = substr($circuit_simulation_udc_row->samplefilepath,strrpos($circuit_simulation_udc_row->samplefilepath, '/'));
		$resource_file =ltrim($str, '/');
			$zip->addFile($root_path . $CIRCUITSIMULATION_PATH . $resource_file , $REFERENCE_PATH . str_replace(' ', '_', basename($resource_file)));
		} //$CIRCUITSIMULATION_udc_row->user_defined_compound_filepath || $CIRCUITSIMULATION_udc_row->user_defined_compound_filepath != 'NULL'
	} //$CIRCUITSIMULATION_udc_row = $CIRCUITSIMULATION_udc_q->fetchObject()
	$query = db_select('esim_circuit_simulation_proposal');
	$query->fields('esim_circuit_simulation_proposal');
	$query->condition('id', $id);
	//$circuit_simulation_f_q = $query->execute();
	/*while ($circuit_simulation_data) {
		$str = substr($circuit_simulation_data->samplefilepath,strrpos($circuit_simulation_data->samplefilepath, '/'));
		$resource_file =ltrim($str, '/');
		$zip->addFile($root_path . $CIRCUITSIMULATION_PATH . $resource_file);
		//var_dump($root_path . $CIRCUITSIMULATION_PATH . $resource_file);die;
	} //$CIRCUITSIMULATION_f_row = $CIRCUITSIMULATION_f_q->fetchObject()
	*/$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $circuit_simulation_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are circuit simulation project in this proposal to download", 'error');
		drupal_goto('circuit-simulation-project/full-download/project');
	}
}