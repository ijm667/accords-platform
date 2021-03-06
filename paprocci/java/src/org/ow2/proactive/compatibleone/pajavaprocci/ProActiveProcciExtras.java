/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2012 by Oasis (INRIA Sophia Antipolis) and ActiveEon teams.     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

package org.ow2.proactive.compatibleone.pajavaprocci;

import java.util.Hashtable;
import java.util.Set;
import java.util.concurrent.*;
import org.apache.log4j.Logger;
import org.ow2.proactive.compatibleone.exchangeobjects.*;
import org.ow2.proactive.compatibleone.misc.*;
import org.ow2.proactive.compatibleone.rm.*;
import org.ow2.proactive.compatibleone.scheduler.*;
import org.ow2.proactive.scripting.*;

/**
 * This class handles the functionalities and formatting of the messages
 * that are obtained from the occi.scheduler.client.SchedulerClient. */
public class ProActiveProcciExtras{
	private static Logger logger =						// Logger. 
			Logger.getLogger(ProActiveProcciExtras.class.getName()); 
	private ProcciProperties props;
	
	public ProActiveProcciExtras(ProcciProperties props){
		this.props = props;
	}
	
	
	/**
	 * Check if the given amount of parameters passed through JNI is correct.
	 * @param args arguments passed to a method through JNI.
	 * @param amount amount of arguments.
	 * @throws Exception if not correct.
	 */
	private void checkparameters(Object[] args, int amount) throws Exception{
		if (args.length != amount){
			throw new Exception("The amount of parameters passed is " + args.length + " but " + amount + " are required.");
		}
	}
	
	/**
	 * Create an executor (thread pool). 
	 * @return executor.
	 */
	private ExecutorService createExecutor(){
		return Executors.newSingleThreadExecutor();
	}
	
	
	/**
	 * Listing of the nodes available in the RM. 
	 * @return a json object with a list of nodes and some info about them. 
	 * @throws Exception in case something goes wrong. 
	 */
	public String list_servers(Object[] args) throws Exception{
		checkparameters(args, 0);
		logger.info("Trying to list the nodes...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				Hashtable<NodeId, String> job2node = scheduler.mapNodesWithJobs();
				ResourceManagerClient rm = ResourceManagerClient.getInstance();
				NodePublicInfoList nodes = rm.getAllTheNodesInfo(job2node);
				String result = nodes.toString();
				scheduler.disconnect();
				rm.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}

	
	/**
	 * Get public information regarding a node. 
	 */
	public String get_server(Object[] args) throws Exception{
		checkparameters(args, 1);
		final String nodeid = args[0].toString();
		logger.info("Trying to get public information of '" + nodeid + "...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				Hashtable<NodeId, String> job2node = scheduler.mapNodesWithJobs();
				ResourceManagerClient rm = ResourceManagerClient.getInstance();
				NodePublicInfo node = rm.getCompleteNodePublicInfo(nodeid, job2node);
				String result = node.toString();
				scheduler.disconnect();
				rm.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}

	/**
	 * Get the output of a task running at a particular node.
	 */
	public String get_node_output(Object[] args) throws Exception{
		checkparameters(args, 1);
		final String jobid = args[0].toString();
		logger.info("Trying to get public information of '" + jobid + "...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				SchedulerClient scheduler = SchedulerClient.getInstance();
				String result = scheduler.acquireOutput(jobid);
				scheduler.disconnect();
				return result;
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}
	
	public String list_os(Object[] args) throws Exception{
		checkparameters(args, 0);
		logger.info("Trying list Operating Systems...");
		Callable<String> callable = new Callable<String>(){
			@Override
			public String call() throws Exception {
				ResourceManagerClient rm = ResourceManagerClient.getInstance(); 
				Set<String> nodeurls = rm.getNodesUrl(false);
				GenerationScript datascript = DataScriptCreator.createScript(DataScriptAddition.getOS());
				//GenerationScript datascript = DataScriptCreator.createScript(DataScriptAddition.getCPUUser());
				rm.runScript(
						nodeurls, 
						datascript);
				rm.disconnect();
				return "";
			}
		};
		
		ExecutorService executor = createExecutor();//Executors.newSingleThreadExecutor();
		Future<String> future = executor.submit(callable); // We ask to execute the callable.
		String res = null;
		try{
			res = future.get(new Integer(props.getProperty("scheduler.timeout")), TimeUnit.SECONDS);
			return res;
		}catch(Exception e){
			logger.warn("Error: ", e);
			return (new ErrorObject(e).toString());
		}
	}
}
